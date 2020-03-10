#include "NeuralNetwork.hpp"

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;

//Constructor:
NeuralNetwork::NeuralNetwork(const std::vector<std::size_t> &);

//Training function:
void NeuralNetwork::train(const MatrixXd & Data, const double alpha, 
                          const size_t niter, const double tolerance){
	//Init:
	double cost{0.};
	L[0]=Data.col(0); //First layer only reads the
	A[0]=L[0];				//input, see doc
	
	//Beginning of the training loop:
	for(size_t t=1; t<=niter; ++t){
		
		/////////////////////////
		// Forward propagation //
		/////////////////////////
		for(size_t l=1; l<nlayers-1; ++l){
			L[l] = A[l-1]*W[l-1] + b[l-1];
			A[l] = tanh(L[l].array());
		}
		//The final output shouldn't be activated, otherwise it will be necessarly a tanh:
		L[nlayers-1] = A[nlayers-2]*W[nlayers-2] + b[nlayers-2];
		A[nlayers-1] = L[nlayers-1];		//this variable will probably be unuseful, 
		                                //storing L[nl] should be enough
		
		//Computing cost as the L2 distance: (divided by 2, for ease in later differentiation)
		cost = .5 * (L[nlayers-1] - Data.col(1)).array().square().matrix().sum();
		
		//Output the current cost: (a --verbose option could be useful)
		if(t%25==0) cout<<"t="<<t<<" cost="<<cost<<"\n";

		//////////////////////////
		// Backward propagation //
		//////////////////////////
		
		//Compute B backward as d(cost)/d(output), no tanh because it's the last layer
		B[0] = L[nlayers-1] - Data.col(1);	
		for(size_t l=0; l<nlayers-2; ++l){
			//Compute gradient of cost wrt W:
			dW[l]=L[nlayers-1]*B[l];
			//Update W:
			W[l]=W[l]-alpha*dW[l];
			//Compute gradient of cost wrt b:
			db[l]=B[l].rowwise().sum();
			//Update b:
			b[l]=b[l]-alpha*db[l];
			//Compute next B: (now there is tanh, and dx[tanh(x)]=1-x^2)
			B[l+1]=(1. - A[l].array().square()) * ( (B[l]*W[nlayers-1-l]).array() );
		}
		
		{ //Updating parameters of the first hidden layer:
			size_t l = nlayers-2;
			dW[l]=L[nlayers-1]*B[l];
			W[l]=W[l]-alpha*dW[l];
			db[l]=B[l].rowwise().sum();
			b[l]=b[l]-alpha*db[l];
		}
		
	
	}//End of the training loop
	
}

