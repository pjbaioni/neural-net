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

void write_set(string filename, const VectorXd & X, const double omega, const double phi, const char & separator = ' '){
	ofstream ofs(filename);
	auto u = [&omega, &phi] (const double & x) {return exp(-x*x*5)*sin(omega*pi*x + phi*pi);};	
	for(size_t i=0; i<X.rows(); ++i)
		ofs<<X(i)<<separator<<u(X(i))<<"\n";
	ofs.close();
}

ostream & help(){
	cout<<"Run with ./a.out [options]\n";
	cout<<"Options:\n-h, --help: print this help\n";
	cout<<"-p, --parameters <filename>: reads parameters from <filename>,\n";
	cout<<"          default filename = \"./../../data/Parameters.pot\"\n ";
	return cout;
}

void init_spacing(VectorXd & X, const size_t n, const size_t xspacing, string & filename){
	switch(xspacing){
		default:
			X.setLinSpaced(n,-1.5,1.5);
			#ifndef NDEBUG
				filename += "Linspaced";
			#endif
			break;
		case 1:
			X = VectorXd::Random(n);
			X = X * 1.5;
			#ifndef NDEBUG
				filename += "Uniform";
			#endif
			break;
		case 2:
		 	default_random_engine gen;
		  normal_distribution<double> dist(0.,.75);
		  auto gaussian = [&dist, &gen] (double) {return dist(gen);};
			X = VectorXd::NullaryExpr(n, gaussian);
			#ifndef NDEBUG
				filename += "Normal";
			#endif
			break;
	}
}

int main(int argc, char** argv){
	GetPot commandline(argc,argv);
	if(commandline.search(2,"-h","--help")){
		help()<<endl;
		return 0;
	}
	const string param_filename = commandline.follow("./../../data/Parameters.pot",2,"-p","--parameters");
	#ifndef NDEBUG
		cout<<"Reading data from "<<param_filename<<endl;
	#endif
	
	GetPot datafile(param_filename.c_str());
	const size_t ntrain = datafile("ntraindata", 70); 
	const size_t ntest = datafile("ntestdata", 30);	
	const size_t xspacing = datafile("xspacing",0); 
	const double omega = datafile("omega",5.); 
	const double phi = datafile("phi",0.); 
	#ifndef NDEBUG	
		cout<<ntrain<<endl;
		cout<<ntest<<endl;
		cout<<xspacing<<endl;
		cout<<omega<<endl;
		cout<<phi<<endl;
	#endif
	
	VectorXd X;
	string filename{"./../../data/TrainingSet" + param_filename.substr(13, param_filename.size()-13-4)};
	init_spacing(X, ntrain, xspacing, filename);
	#ifndef NDEBUG
		filename += to_string(ntrain);
		filename += to_string(omega);
		filename += to_string(phi);
	#endif
	cout<<"Writing "<<filename<<".dat"<<endl;
	write_set(filename+".dat", X, omega, phi);
	
	filename = "./../../data/TestSet" + param_filename.substr(13, param_filename.size()-13-4);
	init_spacing(X, ntest, xspacing, filename);

	#ifndef NDEBUG
		filename += to_string(ntest);
		filename += to_string(omega);
		filename += to_string(phi);
	#endif
	cout<<"Writing "<<filename<<".dat"<<endl;
	write_set(filename+".dat", X, omega, phi);
	
	cout<<"Done"<<endl;
	
	return 0;
}
