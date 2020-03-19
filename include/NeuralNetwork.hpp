#ifndef RETE_NEURALE_HPP
#define RETE_NEURALE_HPP

#include <cstdlib>
#include <vector>
#include <Eigen/Dense>
#include <utility>
#include <limits>
#include <iostream>
#include <memory>

#include "Optimizers.hpp"

typedef Eigen::Matrix< std::size_t, Eigen::Dynamic, 1 > 	VectorXs;

class NeuralNetwork{
private:

	//Hyperparameters:
	std::size_t nlayers;							//total number of layers of the net (hidden+2)
	VectorXs nnodes;									//nnodes[l]=number of nodes of the l-th layer
	
	//Parameters to be optimized:
	std::vector<Eigen::MatrixXd> W;		//Weights, every node has his own weight for every node of the previous layer
	std::vector<Eigen::VectorXd> b;		//Biases, every layer adds a bias to every node of the previous layer
	
	//Forward propagation outputs:
	std::vector<Eigen::MatrixXd> L; 	//L=W*X+b
	std::vector<Eigen::MatrixXd> A;		//A=activation_function(L) (tanh, ReLU...)
	
	//Back propagation gradients:
	std::vector<Eigen::MatrixXd> B;		//B[layer]=d(cost)/d(output[layer]), optional, stored for chain rule, as follows
	std::vector<Eigen::MatrixXd> dW;	//dW[l]=d(cost)/d(W[l])=d(cost)/d(output[l]) * d(output[l])/d(W[l]) = B[l] * ...
	std::vector<Eigen::VectorXd> db;	//db[l]=d(cost)/d(b[l])=...=B*d(output[l])/(db[l])
	
	//Optimizers:
	std::vector<std::shared_ptr<Optimizers::GradientDescent<Eigen::MatrixXd>>> W_optimizer;
	std::vector<std::shared_ptr<Optimizers::GradientDescent<Eigen::VectorXd>>> b_optimizer;
	
public:
	
	//Constructor:
	NeuralNetwork(const VectorXs &);
	
	//Training function:
	void train(const Eigen::MatrixXd & Data, const double alpha, const std::size_t niter, const double tolerance, const size_t W_opt, const size_t b_opt);
	
	//Test function:
	std::pair<Eigen::VectorXd,double> test(const Eigen::MatrixXd & Data);

};


#endif
