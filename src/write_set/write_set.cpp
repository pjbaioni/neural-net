#include <Eigen/Dense>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include "GetPot.hpp"

using namespace std;
using Eigen::VectorXd;


constexpr double pi=4.*std::atan(1.);

void write_set(string filename, const VectorXd & X, const double omega, const char & separator = ' '){
	ofstream ofs(filename);
	auto u = [omega] (const double & x) {return exp(-x*x*5)*sin(omega*pi*x);};	
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

void init_spacing(VectorXd & X, const size_t n, const size_t xspacing, string & filename){
	switch(xspacing){
		default:
			X.setLinSpaced(n,-1.5,1.5);
			filename += "Linspaced";
			break;
		case 1:
			X = VectorXd::Random(n);
			X = X * 1.5;
			filename += "Uniform";
			break;
		case 2:
		 	default_random_engine gen;
		  normal_distribution<double> dist(0.,.75);
		  auto gaussian = [&dist, &gen] (double) {return dist(gen);};
			X = VectorXd::NullaryExpr(n, gaussian);
			filename += "Normal";
			break;
	}
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
	const size_t xspacing = datafile("xspacing",0);
	const double omega = datafile("omega",5.);
	
	string filename{"./../../data/TrainingSet"+to_string(ntrain)};
	VectorXd X;
	init_spacing(X, ntrain, xspacing, filename);
	filename += to_string(omega);
	write_set(filename+".dat", X, omega);
	
	filename = "./../../data/TestSet"+to_string(ntest);
	init_spacing(X, ntest, xspacing, filename);
	filename += to_string(omega);
	write_set(filename+".dat", X, omega);
	
	cout<<"Done"<<endl;
	return 0;
}
