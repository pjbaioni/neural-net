#include <iostream>
#include <Eigen/Dense>
#include <string>

#include "NeuralNetwork.hpp"
#include "GetPot.hpp"
#include "gnuplot-iostream.hpp"

#include <fstream>
#include <sstream>
#include <cassert>

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
	cout<<"Output written on ";
	try{
		cout<<ofsname.substr(5, 50);
  }catch(const std::out_of_range& e){
  	std::cout << "pos exceeds string size\n";
  }
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
	string train_filename{"./../data/TrainingSet"+to_string(ntraindata)};
	size_t ntestdata = datafile("ntestdata", 30);
	string test_filename{"./../data/TestSet"+to_string(ntestdata)};
	string prevision_filename{"./../data/yhat"+to_string(ntestdata)};
	
	const size_t xspacing = datafile("xspacing",0);
	switch(xspacing){
		default:
			train_filename += "Linspaced";
			test_filename += "Linspaced";
			prevision_filename += "Linspaced";
			break;
		case 1:
			train_filename += "Uniform";
			test_filename += "Uniform";
			prevision_filename += "Uniform";
			break;
		case 2:
			train_filename += "Normal";
			test_filename += "Normal";
			prevision_filename += "Normal";
			break;
	}
	const double omega = datafile("omega",5.);
	const double phi = datafile("phi",0.);
	train_filename += to_string(omega); train_filename += to_string(phi);
	test_filename += to_string(omega); test_filename += to_string(phi);
	prevision_filename += to_string(omega); prevision_filename += to_string(phi);
	
	size_t nlayers = datafile("nlayers", 9);
	string architecture_filename {"./../data/architecture"+to_string(nlayers)};
	double alpha = datafile("alpha",1e-2);
	size_t niter = datafile("niter",250000);
	double tol = datafile("tol",1e-4);
	size_t W_opt = datafile("W_opt",4);
	size_t b_opt = datafile("b_opt",3);
	size_t nref = datafile("nref",3);
	
	//Load the training data:
	MatrixXd TrainData(ntraindata,2);
	bool read=read_set(train_filename+".dat",TrainData);
	if(!read) return -1;

	//Load the net architecture:
	VectorXs architecture(nlayers);
	read=read_set(architecture_filename+".dat",architecture);
	assert(read);
	
	//Construct the net:
	NeuralNetwork nn(architecture);
	
	//Train the net:
	nn.train(TrainData,alpha,niter,W_opt,b_opt,tol,nref,verbose);
	
	//Load the test data:
	MatrixXd TestData(ntestdata,2);
	read=read_set(test_filename+".dat",TestData);
	assert(read);
	
	//Test the net:
	VectorXd yhat;
	double errL2;
	tie(yhat,errL2)=nn.test(TestData);
	
	//Print the results:
	cout<<"Relative L2 error on test set = "<<errL2<<endl;
	write_vector(prevision_filename+".dat",yhat)<<"\n"<<endl;
	
	Gnuplot gp;
	//first way:
	vector<double> xtest(TestData.rows()),ytest(TestData.rows());
	VectorXd::Map(&xtest[0], ntestdata) = TestData.col(0);
	VectorXd::Map(&ytest[0], ntestdata) = TestData.col(1);
	//second way
	vector<double> prevision(yhat.data(), yhat.data() + yhat.rows()*yhat.cols());
	//plot:
	xspacing == 0 ?
	gp<<"plot"<<gp.file1d(std::tie(xtest,ytest))<<
  "w lp lw 4 title 'Test Data',"<< gp.file1d(std::tie(xtest,prevision))<<
  "w lp lw 1.5 title 'Prevision'"<<endl :
	gp<<"plot"<<gp.file1d(std::tie(xtest,ytest))<<
  "pt 7 title 'Test Data',"<< gp.file1d(std::tie(xtest,prevision))<<
  "pt 3 title 'Prevision'"<<endl;
	
	return 0;
} 
