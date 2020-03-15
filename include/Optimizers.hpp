#ifndef OTTIMIZZATORI_HPP
#define OTTIMIZZATORI_HPP

#include <cstdlib>
#include <cmath>
#include <Eigen/Dense>

namespace Optimizers{

/*
In the following T is meant to be a Eigen matrix type.
It is necessary to work both with VectorXd and MatrixXd.
Maybe it's better to use something like:
	template <typename Derived>
	void function_name( const MatrixBase<Derived> &)
Moreover inheritance should be considered, both for the evident inherited structure
and for polymorfism and runtime choices
*/

template<typename T>
class GradientDescent{
public:
	void operator()(T& theta, const T& gt, const double& alpha){
		theta=theta-alpha*gt;
	}
};

template<typename T>
class GDwithMomentum{
private:
	T mt;
	double beta{0.9};
public:
	Momentum(const size_t & m, const size_t &n):mt(m,n){}
	void set_beta(double b){beta=b;}
	
	void operator()(T& theta, const T& gt, const double& alpha, const double & t){
		mt=beta1*mt + (1.0-beta1)*gt;
		theta=theta-alpha*mt;
	}
};

template<typename T>
class RMSprop{
private:
	T st;
	double beta2{0.9};
public:
	RMSprop(const size_t & m, const size_t & n):st(m,n){}
	void set_beta(double b){beta=b;}
	
	void operator()(T& theta, const T& gt, const double& alpha, const double & t){
		st=beta*st + (1. - beta)*(gt.cwiseProduct(gt));
		theta=theta - alpha * ( gt.cwiseProduct( 1./(1e-8+sqrt(st.array())) ));
	}
};

template<typename T>
class Adam{
private:
	T mt;
	T vt;
	
	double beta1=0.9;
	double beta2=0.999;
	double epsilon=1e-8;
public:
	Adam(const size_t & m, const size_t & n): mt(m,n), vt(m,n){}
	void set_param(double b1, double b2, double e){
		beta1=b1; beta2=b2; epsilon=e;
	}

	//This follows the Kingma-Ba 2015 article,
	//consider possible memory savings...
	void operator()(T& theta, const T& gt, const double & alpha, const size_t & t){
		//compute the momenta:
		mt = beta1*mt + (1.0-beta1)*gt; //GDwithMomentum step
    vt = beta2*vt + (1.0-beta2)*(gt.cwiseProduct(gt));	//RMSprop step
		//correction step:
    T mt_hat = (1.0/(1.0-pow(beta1,t)))*mt;
    T vt_hat = (1.0/(1.0-pow(beta2,t)))*vt;
		//update:
    theta=theta-(alpha * ( mt_hat.cwiseProduct( ( 1./(epsilon + sqrt(vt_hat.array())) ).matrix() )));
	}
};

template<typename T>
class AdaMax{
private:
	T mt;
	T ut;
	
	double beta1=0.9;
	double beta2=0.999;
	double epsilon=1e-8;
	
	T cwiseMax(const T& a, const T& b){
		//both T must have same dimensions,
		//implement a check
		T ret(a.rows(),a.cols());
		for(size_t i=0; i<a.rows(); ++i)
			for(size_t j=0; j<a.cols();++j)
				ret(i,j)=std::max(a(i,j),b(i,j));
		return ret;
	}
	
public:
	AdaMax(const size_t & m, const size_t & n): mt(m,n), ut(m,n){}
	void set_param(double b1, double b2, double e){
		beta1=b1; beta2=b2; epsilon=e;
	}
	
	//Memory already saved here,
	//since AdaMax doesn't require an Adam-like correction step
	void operator()(T& theta, const T& gt, const double & alpha, const size_t & t){
		//compute the momenta
		mt = beta1*mt + (1.0-beta1)*gt; //GDwithMomentum step
		ut = cwiseMax(beta2*ut,gt.cwiseAbs()); //AdaMax step
		//update:
    theta=theta - ( alpha / (1.0-pow(beta1,t)) ) * ( mt.cwiseProduct( (1./ut.array()).matrix() ) );
	}
};

}//End of Optimizers namespace

#endif
