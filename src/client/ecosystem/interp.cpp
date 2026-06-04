#include "interp.hpp"
#include "engine.hpp"
#include "console.hpp"
std::vector<Interpolation> Interpolator::interpolations;

void Interpolator::add(float &target, int min, int max, int t, int lapse) {
	interpolations.push_back(Interpolation(target, min, max, t, lapse ));
}

void Interpolator::update() {
	Console::clear();
	std::vector<Interpolation>::iterator it=interpolations.begin();
	for (; it < interpolations.end();it++){		
		if (it->t >= it->lapse) {
			interpolations.erase(it);
			return;
		} else {
			*(it->var) += it->delta;
			it->t++;
			Engine::printf("%d/%d %d ",it->t, it->lapse, (int)*(it->var));
		}
	}	
}
