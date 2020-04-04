#include "NeuralNetwork.hpp"

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using namespace Optimizers;

//Constructor:
NeuralNetwork::NeuralNetwork(const VectorXs & nn): nlayers(nn.rows()),nnodes(nn){
	//In same the order of the declaration,
	//mind that, since ndata is unknown here,
	//L[l],A[l],B[l] have to be defined later, before the training loop
	//same for optimizer, which are still unknown
	
	W.reserve(nlayers-1);
	b.reserve(nlayers-1);
	for(size_t l=0; l<nlayers-1; ++l){
		W.emplace_back(MatrixXd::Random(nnodes(l),nnodes(l+1)));	//W MUST be random initialized, see doc
		b.emplace_back(VectorXd(nnodes(l+1)));
	}
	
	L.reserve(nlayers);	 //--> at last layer B=L-Y, not A-Y (even if A==L), 
	A.reserve(nlayers-1); // bcs A will be out of range
	
	B.reserve(nlayers-1);	
	dW=W; //1) this require that dW[l] and W[l] refer to the same l, same for b
	db=b; //2) first value isn't important here, since it would be overwritten anyway 
	
	W_optimizer.reserve(nlayers-1);
	b_optimizer.reserve(nlayers-1);
}

//Training function:
void NeuralNetwork::train(const MatrixXd & Data, double alpha, size_t niter, const size_t W_opt,
                          const size_t b_opt, double tolerance, const size_t nrefinements, const bool verbose){                     
	///////////////////
	//      Init     //
	///////////////////
	
	size_t ndata=Data.rows();
	
	//L has nlayers components
	for(size_t l=0; l<nlayers;++l)
		L.emplace_back(ndata,nnodes(l));
	//A hasn't the last one
	for(size_t l=0; l<nlayers-1;++l)
		A.emplace_back(ndata,nnodes(l));
	//B hasn't the first one
	for(size_t l=1; l<nlayers;++l)
		B.emplace_back(ndata,nnodes(l));
	
	//First layer only reads the input, see doc
	L[0]=Data.col(0); 
	A[0]=L[0];	
	
	//Optimizers:
	//Preliminar tests has shown that the best compromise between accuracy
	//and number of iteration is reached when using AdaMax for W and Adam for b,
	//so these are the default
	string W_opt_name, b_opt_name;
	switch(W_opt){
		case 0:
			for(size_t l=0; l<nlayers-1; ++l)
				W_optimizer.emplace_back(make_shared<GradientDescent<MatrixXd>>(dW[l].rows(),dW[l].cols()));
			W_opt_name = "GradientDescent";
			break;
		case 1:
			for(size_t l=0; l<nlayers-1; ++l)
				W_optimizer.emplace_back(make_shared<GDwithMomentum<MatrixXd>>(dW[l].rows(),dW[l].cols()));
			W_opt_name = "GDwithMomentum";
			break;
		case 2:
			for(size_t l=0; l<nlayers-1; ++l)
				W_optimizer.emplace_back(make_shared<RMSprop<MatrixXd>>(dW[l].rows(),dW[l].cols()));
			W_opt_name = "RMSprop";
			break;
		case 3:
			for(size_t l=0; l<nlayers-1; ++l)
				W_optimizer.emplace_back(make_shared<Adam<MatrixXd>>(dW[l].rows(),dW[l].cols()));
			W_opt_name = "Adam";
			break;
		default:
			for(size_t l=0; l<nlayers-1; ++l)
				W_optimizer.emplace_back(make_shared<AdaMax<MatrixXd>>(dW[l].rows(),dW[l].cols()));
			W_opt_name = "AdaMax";
			break;
	}		
	
	switch(b_opt){
		case 0:
			for(size_t l=0; l<nlayers-1; ++l)
				b_optimizer.emplace_back(make_shared<GradientDescent<VectorXd>>(db[l].rows(),db[l].cols()));
			b_opt_name = "GradientDescent";
			break;
		case 1:
			for(size_t l=0; l<nlayers-1; ++l)
				b_optimizer.emplace_back(make_shared<GDwithMomentum<VectorXd>>(db[l].rows(),db[l].cols()));
			b_opt_name = "GDwithMomentum";
			break;
		case 2:
			for(size_t l=0; l<nlayers-1; ++l)
				b_optimizer.emplace_back(make_shared<RMSprop<VectorXd>>(db[l].rows(),db[l].cols()));
			b_opt_name = "RMSprop";
			break;
		default:
			for(size_t l=0; l<nlayers-1; ++l)
				b_optimizer.emplace_back(make_shared<Adam<VectorXd>>(db[l].rows(),db[l].cols()));
			b_opt_name = "Adam";
			break;
		case 4:
			for(size_t l=0; l<nlayers-1; ++l)
				b_optimizer.emplace_back(make_shared<AdaMax<VectorXd>>(db[l].rows(),db[l].cols()));
			b_opt_name = "AdaMax";
			break;
	}	
	
	///////////////////
	//Training loops //
	///////////////////
	double old_cost{numeric_limits<double>::infinity()};
	double cost{-1.}, err{old_cost};
	niter=niter/nrefinements;
	vector<size_t> backup_t(nrefinements);
	
	for(size_t ref=1; ref<=nrefinements; ++ref){
		
		for(size_t t=1; t<=niter; ++t){

			/////////////////////////
			// Forward propagation //
			/////////////////////////
			for(size_t l=1; l<nlayers-1; ++l){
				//Summing the column vector b (nnodes(l+1)x1) to each 
				//column of A*W (ndataxnnodes(l)*nnodes(l)xnnodes(l+1)) :
				L[l] = ( A[l-1]*W[l-1] ).rowwise() + b[l-1].transpose();
				A[l] = tanh( L[l].array() );
			}	
			//The final output shouldn't be activated, otherwise it will be necessarly a tanh:
			L[nlayers-1] = ( A[nlayers-2]*W[nlayers-2] ).rowwise() + b[nlayers-2].transpose();
		
			//Computing cost as the L2 distance: (divided by 2, for ease in later differentiation)
			cost = .5 * (L[nlayers-1] - Data.col(1)).array().square().matrix().sum();
		
		
			/////////////////////////
			//  Convergence check  //
			/////////////////////////
			//Output the current cost
			//and check if convergence is reached:
			
			if( t%100==0 ){
				if(verbose) 
					cout<<"t="<<t<<" cost="<<cost<<" W_opt="<<W_opt_name<<" b_opt="<<b_opt_name<<" alpha="<<alpha<<"\n";
				err = abs(old_cost-cost) / ( (cost+old_cost)/2 );
				if(err<tolerance){
					backup_t[ref-1]=t;
					break;
				}
				else
					old_cost = cost;
			}

			//////////////////////////
			// Backward propagation //
			//////////////////////////
			//Compute B as d(cost)/d(output): (no tanh now because it's the last layer)
			B[nlayers-2] = L[nlayers-1] - Data.col(1);	
			for(size_t l=nlayers-2; l>0; --l){
				//Compute gradient of cost wrt W:
				dW[l] = ( L[l].transpose() )*B[l];
				//Update W:
				//W[l] = W[l] - alpha*dW[l];
				(*W_optimizer[l])(W[l],dW[l],alpha,t);
				//Compute gradient of cost wrt b:
				db[l] = B[l].transpose().rowwise().sum();
				//Update b:
				//b[l] = b[l] - alpha*db[l];
				(*b_optimizer[l])(b[l],db[l],alpha,t);
				//Compute previous B: (now there is tanh, and dx[tanh(x)]=1-x^2)
				B[l-1] = (1. - (A[l].array().square())) * ( (B[l]* (W[l].transpose()) ).array() );
			}
			//Updating parameters of the first hidden layer:
				dW[0] = ( L[0].transpose() )*B[0];
				W[0] = W[0] - alpha*dW[0];
				db[0] = B[0].transpose().rowwise().sum();
				b[0] = b[0] - alpha*db[0];
	
		}//End of the training loop
	
	//update refinement parameters for the next training loop:
	alpha=alpha/10;
	tolerance=tolerance/((10-2*ref)*10);
	}//End of the refinements loop

	//////////////////////////
	//     Final output     //
	//////////////////////////
	size_t total_iter=0;
	for(const auto & r: backup_t)
		r == 0 ? total_iter += niter : total_iter += r;
	cout<<"Total iterations = "<<total_iter<<"\n"<<"Cost functional on the training set = "<<cost<<endl;
		
}//End of the train function

//Test function:
pair<VectorXd,double> NeuralNetwork::test(const MatrixXd & Data){
	
	//One forprop as before during training,
	//w/o storing anything but the final result:
	MatrixXd Ltest{Data.col(0)};
	MatrixXd Atest{Ltest};
	
	for(size_t l=1; l<nlayers-1; ++l){
		Ltest = ( Atest*W[l-1] ).rowwise() + b[l-1].transpose();
		Atest = tanh( Ltest.array() );	
	}
	Ltest = ( Atest*W[nlayers-2] ).rowwise() + b[nlayers-2].transpose();
	
	//Computation of the L2 relative error:
	double numerator = ( Data.col(1)-Ltest ).norm();
	double denominator = ( Data.col(1).norm() + Ltest.norm() )/2.;
	
	
	return make_pair(Ltest, (numerator/denominator) );
}
