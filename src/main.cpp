#include <iostream>
#include <Eigen/Dense>
#include <string>

#include "NeuralNetwork.hpp"

//The following have to be removed once i/o stuff
//will be defined in proper files
#include <fstream>
#include <sstream>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;

//To be moved:
template <typename T> //T has to be a matrix which overloads the call operator for indexing
void read_set(const string& setname, T & M, const char & separator=' '){
	ifstream in(setname);
	if(!in) cerr<<"Error in opening: "<<setname<<std::endl; //add a proper exit...try,catch...or just go to default
	string riga,numero;
	for(size_t i=0; getline(in,riga); ++i){
		istringstream rigastream(riga);
		for(size_t j=0; getline(rigastream,numero,separator); ++j)
			M(i,j)=stod(numero);
	}
}

ostream & write_vector(const string & ofsname, const VectorXd & X){
	ofstream out_file(ofsname);
	streambuf* stream_buffer_cout = cout.rdbuf();
	streambuf* stream_buffer_file = out_file.rdbuf();
	cout.rdbuf(stream_buffer_file);
	cout<<X;
	out_file.close();
	cout.rdbuf(stream_buffer_cout);
	cout<<"Output written on "<<ofsname;
	return cout;
} 

//To be kept:
int main(int argc, char** argv){
	
	//Get data:
	size_t ntraindata{70},nlayers{9},ntestdata{30};
	double alpha{1e-3},tol{1e-3};
	size_t niter{25000};
	string train_filename,architecture_filename, test_filename;
	//use GetPot here...
	
	//Load the training data:
	MatrixXd TrainData(ntraindata,2);
	read_set("./../data/Train.csv",TrainData,',');

	//Load the net architecture:
	VectorXs architecture(nlayers);
	//read_set("architecture.dat",architecture);
	architecture(0)=1;
	architecture(1)=3;
	architecture(2)=5;
	architecture(3)=7;
	architecture(4)=9;
	architecture(5)=7;
	architecture(6)=5;
	architecture(7)=3;
	architecture(8)=1;
	
	//Construct the net:
	NeuralNetwork nn(architecture);
	
	//Train the net:
	nn.train(TrainData,alpha,niter,tol);
	
	//Load the test data:
	MatrixXd TestData(ntestdata,2);
	read_set("./../data/Test.csv",TestData,',');
	
	//Test the net:
	VectorXd Yhat;
	double errL2;
	tie(Yhat,errL2)=nn.test(TestData);
	
	//Print the results:
	cout<<"Relative L2 error on test set = "<<errL2<<endl;
	write_vector("./../data/Yhat.dat",Yhat)<<endl;
	
	return 0;
} 
