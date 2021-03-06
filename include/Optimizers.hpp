#ifndef OTTIMIZZATORI_HPP
#define OTTIMIZZATORI_HPP

#include <cstdlib>
#include <cmath>
#include <Eigen/Dense>
#include <cassert>

namespace Optimizers{

template<typename T>
class GradientDescent{
protected:
	virtual void update_objective(T& theta, const T& delta_theta, const double& alpha){
		theta = theta - alpha*delta_theta;  
	}
public:
	GradientDescent()=default;
	GradientDescent(const size_t & m, const size_t & n):GradientDescent(){}
	virtual ~GradientDescent()=default;
	virtual void operator()(T& theta, const T& gt, const double& alpha, const std::size_t & t){
		update_objective(theta,gt,alpha);
	}
};

template<typename T>
class GDwithMomentum : public GradientDescent<T>{
protected:
	T mt;
	double beta{0.9};
	
	virtual void compute_momentum(const T & to_be_averaged){
		mt = beta*mt + (1. - beta)*to_be_averaged;
	}
public:
	GDwithMomentum(const size_t & m, const size_t &n):GradientDescent<T>::GradientDescent(m,n),mt(m,n){}
	virtual void set_beta(double b){beta=b;}

	void operator()(T& theta, const T& gt, const double& alpha, const std::size_t & t) override {
		compute_momentum(gt);
		this->update_objective(theta,mt,alpha);
	}
};

template<typename T>
class Adam;

template<typename T>
class RMSprop final : public GDwithMomentum<T>{
private:
	double epsilon{1e-8};
	
	friend class Adam<T>;
public:
	RMSprop(const size_t & m, const size_t & n):GDwithMomentum<T>::GDwithMomentum(m,n){}
	virtual void set_eps(double e){epsilon=e;}
	
	void operator()(T& theta, const T& gt, const double& alpha, const std::size_t & t) override {
		this->compute_momentum(gt.cwiseProduct(gt));
		this->update_objective(theta, ( gt.cwiseProduct( (1./(epsilon + sqrt(this->mt.array()))).matrix() )), alpha);
	}
};

template<typename T>
class KingmaBa : public GDwithMomentum<T>{
protected:
	double beta2{0.999};
public:
	KingmaBa(const size_t & m, const size_t & n):GDwithMomentum<T>::GDwithMomentum(m,n){}
	virtual void set_beta2(double b2){ beta2=b2;}

	void operator()(T& theta, const T& gt, const double & alpha, const std::size_t & t)override = 0;
};

template<typename T>
class Adam final : public KingmaBa<T>{
private:
	RMSprop<T> rms;
	T correction_step(const double beta, const std::size_t t, const T & mt){
		return ((1.0/(1.0-std::pow(beta,t)))*mt); //std::pow faster then for loop based "mypow()", see doc
	}
public:
	Adam(const size_t & m, const size_t & n): KingmaBa<T>::KingmaBa(m,n), rms(m,n){}
	
	void operator()(T& theta, const T& gt, const double& alpha, const std::size_t & t)override{
		//compute the momenta:
		this->compute_momentum(gt);	//GDwithMomentum step
		rms.set_beta(this->beta2);
		rms.compute_momentum(gt.cwiseProduct(gt));//RMSprop step
	//Strictly following the Kingma-Ba 2015 article
		//correction step:
		//T mt_hat = correction_step(this->beta,t,this->mt);
		//T vt_hat = correction_step(this->beta2,t,rms.get_st());
		//update:
		//this->update_objective(theta,( mt_hat.cwiseProduct( ( 1./(rms.get_eps() + sqrt(vt_hat.array())) ).matrix() )), alpha);
	//Adding some memory saving:
		this->update_objective(theta,( (correction_step(this->beta,t,this->mt)).cwiseProduct( ( 1./(rms.epsilon + sqrt((correction_step(this->beta2,t,rms.mt)).array())) ).matrix() )), alpha);
	}	

};

template<typename T>
class AdaMax final : public KingmaBa<T>{
private:
	T ut;
	
	T cwiseMax(const T& a, const T& b){
		#ifndef NDEBUG
			bool AdaMaxMatricesDimensionsMatch = (a.rows()==b.rows() && a.cols()==b.cols());
			assert(AdaMaxMatricesDimensionsMatch);
		#endif
		T ret(a.rows(),a.cols());
		for(size_t i=0; i<a.rows(); ++i)
			for(size_t j=0; j<a.cols();++j)
				ret(i,j)=std::max(a(i,j),b(i,j));
		return ret;
	}
	
	double epsilon{1e-8};
	
public:
	AdaMax(const size_t & m, const size_t & n): KingmaBa<T>::KingmaBa(m,n), ut(m,n){}
	
	void set_epsilon(double e){epsilon=e;}
	
	void operator()(T& theta, const T& gt, const double& alpha, const std::size_t & t)override{
		//compute the momenta:
		this->compute_momentum(gt);										//GDwithMomentum step
		ut = cwiseMax(this->beta2*ut,gt.cwiseAbs());	//AdaMax step
		//update:
		this->update_objective(theta, (( 1. / (1.-std::pow(this->beta,t)) ) * ( this->mt.cwiseProduct( (1./(epsilon+ut.array())).matrix() ) )), alpha);
	}


};

}//End of Optimizers namespace

#endif
