#include "NeuralNetwork.hpp"

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;

//Constructor:
NeuralNetwork::NeuralNetwork(const VectorXs & nn): nlayers(nn.rows()),nnodes(nn){
	//In same the order of the declaration,
	//mind that, since ndata is unknown here,
	//L[l],A[l],B[l] have to be defined later, before the training loop
	
	W.reserve(nlayers-1);
	b.reserve(nlayers-1);
	for(size_t l=0; l<nlayers-1; ++l){
		W.push_back(MatrixXd::Random(nnodes(l),nnodes(l+1)));	//W MUST be random initialized, see doc
		b.push_back(VectorXd(nnodes(l+1)));
	}
	
	L.resize(nlayers);	 //--> at last layer B=L-Y, not A-Y (even if A==L), 
	A.resize(nlayers-1); // bcs A will be out of range
	
	B.resize(nlayers-1);	
	dW=W; //1) this require that dW[l] and W[l] refer to the same l, same for b
	db=b; //2) first value isn't important here, since it would be overwritten anyway 
	
	W_opt.reserve(nlayers-1);
	b_opt.reserve(nlayers-1);
	//the following is meaningful only if it exist
	//a constructor taking two unsigned
	for(size_t l=0; l<nlayers-1; ++l){
		W_opt.emplace_back(dW[l].rows(),dW[l].cols());
		b_opt.emplace_back(db[l].rows(),db[l].cols());
	}
}

//Training function:
void NeuralNetwork::train(const MatrixXd & Data, const double alpha, 
                          const size_t niter, const double tolerance){                     
	///////////////////
	//      Init     //
	///////////////////
	
	size_t ndata=Data.rows();
	double old_cost{numeric_limits<double>::infinity()};
	double cost{-1.}, err{old_cost};
	
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
	
	
	//Beginning of the training loop:
	for(size_t t=1; t<=niter; ++t){
		//char temp;
		/////////////////////////
		// Forward propagation //
		/////////////////////////
		for(size_t l=1; l<nlayers-1; ++l){
			//Summing the column vector b (nnodes(l+1)x1) to each 
			//column of A*W (ndataxnnodes(l)*nnodes(l)xnnodes(l+1)) :
			L[l] = ( A[l-1]*W[l-1] ).rowwise() + b[l-1].transpose();
			A[l] = tanh( L[l].array() );
			//if(t>=2) {cout<<A[l]<<"\n\n"<<L[l]<<"\n\n"<<t<<endl; cin>>temp;}
		}	
		//The final output shouldn't be activated, otherwise it will be necessarly a tanh:
		L[nlayers-1] = ( A[nlayers-2]*W[nlayers-2] ).rowwise() + b[nlayers-2].transpose();
		
		//Computing cost as the L2 distance: (divided by 2, for ease in later differentiation)
		cost = .5 * (L[nlayers-1] - Data.col(1)).array().square().matrix().sum();
		
		//Output the current cost (a --verbose option could be useful)
		//and check if convergence is reached:
		if(t%25==0){
			cout<<"t="<<t<<" cost="<<cost<<"\n";
			err = abs(old_cost-cost) / ( (cost+old_cost)/2 );
			if(err<tolerance)
				return;
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
			W_opt[l](W[l],dW[l],alpha,t);
			//Compute gradient of cost wrt b:
			db[l] = B[l].transpose().rowwise().sum();
			//Update b:
			//b[l] = b[l] - alpha*db[l];
			b_opt[l](b[l],db[l],alpha,t);
			//Compute previous B: (now there is tanh, and dx[tanh(x)]=1-x^2)
			B[l-1] = (1. - (A[l].array().square())) * ( (B[l]* (W[l].transpose()) ).array() );
		}
		//Updating parameters of the first hidden layer:
			dW[0] = ( L[0].transpose() )*B[0];
			W[0] = W[0] - alpha*dW[0];
			db[0] = B[0].transpose().rowwise().sum();
			b[0] = b[0] - alpha*db[0];
	
	}//End of the training loop
	
}

//Test function:
pair<VectorXd,double> NeuralNetwork::test(const Eigen::MatrixXd & Data){
	
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
