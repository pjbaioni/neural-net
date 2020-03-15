#include <Eigen/Dense>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>

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
	const size_t ntrain=70;
	const size_t ntest=30;
	VectorXd X;
	X.setLinSpaced(ntrain,-1.5,1.5);
	string file1{"./../data/LinspacedTrainingSet.dat"}, file2{"./../data/LinspacedTestSet.dat"};
	write_set(file1,X);
	X.setLinSpaced(ntest,-1.5,1.5);
	write_set(file2,X);
	return 0;
}
