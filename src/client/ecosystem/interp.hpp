#ifndef __INTERPOLATOR_HPP
#define __INTERPOLATOR_HPP

#include <vector>

class Interpolation {
public:
	float *var;
	float delta = 0.0f;	
	int min = 1;	
	int max = 0;
	int t	= 0;	// 'when' is it right now 
	int lapse = 60;	//how long the interpolation will be
	Interpolation(float &var, int min = 1, int max = 0, int t = 0, int lapse = 60) {
		this->var=&var;
		this->delta = delta;
		this->min = min;
		this->max = max;
		this->t = t;
		this->lapse = lapse;
		delta = (float(max - min) / lapse);
		var = min + (delta * t);	
	}
};

class Interpolator {
	private:	
		static std::vector<Interpolation> interpolations;
	public:
		static void add(float &target, int min=0, int max=1, int t=0, int lapse=60);
		static void update();
};

#endif
