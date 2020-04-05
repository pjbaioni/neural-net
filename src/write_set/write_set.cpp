#include <Eigen/Dense>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "GetPot.hpp"

using namespace std;
using Eigen::VectorXd;

//compile with:
//g++ -std=c++14 -I$mkEigenInc -I./../../include/ write_set.cpp


constexpr double pi=4.*std::atan(1.);

void write_set(string filename, const VectorXd & X, const char & separator = ' '){
	ofstream ofs(filename);
	auto u = [] (const double & x) {return exp(-x*x*5)*sin(5*pi*x);};	
	for(size_t i=0; i<X.rows(); ++i)
		ofs<<X(i)<<separator<<u(X(i))<<"\n";
	ofs.close();
}

ostream & help(){
	cout<<"Run with ./a.out [options]\n";
	cout<<"Options:\n-h, --help: print this help\n";
	cout<<"-p, --parameters <filename>: reads parameters from <filename>,\n";
	cout<<"          default filename = \"./../../data/parameters.pot\"\n ";
	return cout;
}


int main(int argc, char** argv){
	GetPot commandline(argc,argv);
	if(commandline.search(2,"-h","--help")){
		help()<<endl;
		return 0;
	}
	string param_filename = commandline.follow("./../../data/parameters.pot",2,"-p","--parameters");
	GetPot datafile(param_filename.c_str());
	const size_t ntrain = datafile("ntraindata", 70);
	const size_t ntest = datafile("ntestdata", 30);
	VectorXd X;
	X.setLinSpaced(ntrain,-1.5,1.5);
	string file1{"./../../data/TrainingSet"+to_string(ntrain)}, file2{"./../../data/TestSet"+to_string(ntest)};
	write_set(file1+".dat",X);
	X.setLinSpaced(ntest,-1.5,1.5);
	write_set(file2+".dat",X);
	cout<<"Done"<<endl;
	return 0;
}
