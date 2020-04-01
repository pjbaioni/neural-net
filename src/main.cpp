#include <iostream>
#include <Eigen/Dense>
#include <string>

#include "NeuralNetwork.hpp"
#include "GetPot.hpp"
#include "gnuplot-iostream.hpp"

#include <fstream>
#include <sstream>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;

template <typename T> //T has to be a matrix which overloads the call operator for indexing
bool read_set(const string& setname, T & M, const char & separator=' '){
	ifstream in(setname);
	if(!in){
		cerr<<"Error in opening: "<<setname<<std::endl;
		return false;
	}
	string riga,numero;
	for(size_t i=0; getline(in,riga); ++i){
		istringstream rigastream(riga);
		for(size_t j=0; getline(rigastream,numero,separator); ++j)
			M(i,j)=stod(numero);
	}
	return true;
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


ostream & help(){
	cout<<"Run with ./main.out [options]\n";
	cout<<"Options:\n-h, --help: print this help\n-v, --verbose: activate verbose mode\n";
	cout<<"-p, --parameters <filename>: reads parameters from <filename>,\n";
	cout<<"          default filename = \"./../data/parameters.pot\"\n ";
	return cout;
}

int main(int argc, char** argv){
	
	//Get data:
	GetPot commandline(argc,argv);
	string param_filename = commandline.follow("./../data/parameters.pot",2,"-p","--parameters");
	if(commandline.search(2,"-h","--help")){
		help()<<endl;
		return 0;
	}
	cout<<endl;
	bool verbose = commandline.search(2,"-v","--verbose");
	
	GetPot datafile(param_filename.c_str());
	size_t ntraindata = datafile("ntraindata", 70);
	string train_filename = datafile("train_filename", "./../data/LinspacedTrainingSet70.dat");
	size_t ntestdata = datafile("ntestdata", 30);
	string test_filename = datafile("test_filename", "./../data/LinspacedTestSet300.dat");
	size_t nlayers = datafile("nlayers", 9);
	string architecture_filename = datafile("architecture_filename", "./../data/architecture.dat");
	double alpha = datafile("alpha",1e-2);
	size_t niter = datafile("niter",500000);
	double tol = datafile("tol",1e-4);
	size_t W_opt = datafile("W_opt",4);
	size_t b_opt = datafile("b_opt",3);
	size_t nref = datafile("nref",3);
	string prevision_filename = datafile("prevision_filename", "./../data/yhat300.dat");
	
	//Load the training data:
	MatrixXd TrainData(ntraindata,2);
	bool read=read_set(train_filename,TrainData);
	if(!read) return -1;

	//Load the net architecture:
	VectorXs architecture(nlayers);
	read=read_set(architecture_filename,architecture);
	if(!read) return -1;
	
	//Construct the net:
	NeuralNetwork nn(architecture);
	
	//Train the net:
	nn.train(TrainData,alpha,niter,tol,W_opt,b_opt,nref,verbose);
	
	//Load the test data:
	MatrixXd TestData(ntestdata,2);
	read=read_set(test_filename,TestData);
	if(!read) return -1;
	
	//Test the net:
	VectorXd yhat;
	double errL2;
	tie(yhat,errL2)=nn.test(TestData);
	
	//Print the results:
	cout<<"Relative L2 error on test set = "<<errL2<<endl;
	write_vector(prevision_filename,yhat)<<"\n"<<endl;
	
	Gnuplot gp;
	//first way:
	vector<double> xtest(TestData.rows()),ytest(TestData.rows());
	VectorXd::Map(&xtest[0], ntestdata) = TestData.col(0);
	VectorXd::Map(&ytest[0], ntestdata) = TestData.col(1);
	//second way
	vector<double> prevision(yhat.data(), yhat.data() + yhat.rows()*yhat.cols());
	//plot:
	gp<<"plot"<<gp.file1d(std::tie(xtest,ytest))<<
  "w lp lw 4 title 'Test Data',"<< gp.file1d(std::tie(xtest,prevision))<<
  "w lp lw 1.5 title 'Prevision'"<<endl;
	
	return 0;
} 
