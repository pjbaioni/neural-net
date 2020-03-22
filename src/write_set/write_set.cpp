#include <Eigen/Dense>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>

#include "GetPot.hpp"

using namespace std;
using Eigen::VectorXd;

constexpr double pi=4.*std::atan(1.);

void write_set(string filename, const VectorXd & X, const char & separator = ' '){
	ofstream ofs(filename);
	auto u = [] (const double & x) {return exp(-x*x*5)*sin(5*pi*x);};	
	for(size_t i=0; i<X.rows(); ++i)
		ofs<<X(i)<<separator<<u(X(i))<<"\n";
	ofs.close();
}

int main(){
	GetPot datafile("./../../data/parameters.pot");
	const size_t ntrain = datafile("ntraindata", 70);
	const size_t ntest = datafile("ntestdata", 30);
	VectorXd X;
	X.setLinSpaced(ntrain,-1.5,1.5);
	string file1{"./../../data/LinspacedTrainingSet"+to_string(ntrain)}, file2{"./../../data/LinspacedTestSet"+to_string(ntest)};
	write_set(file1+".dat",X);
	X.setLinSpaced(ntest,-1.5,1.5);
	write_set(file2+".dat",X);
	return 0;
}
