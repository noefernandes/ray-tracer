#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "../scene.h"
#include "light.hpp"
#include "../vec3.hpp"
#include <cmath>

class DirectionalLight : public Light {
	public:
		vec3<float> l;
		vec3<float> scale;
		vec3<float> to;

		DirectionalLight(std::string type, vec3<float> l, vec3<float> scale, vec3<float> from, 
		vec3<float> to) : Light(type, from){
			this->type = type;
			this->l = l;
			this->scale = scale;
			this->from = from;
			this->to = to;
		}

		void preprocess(Scene& scene){}

		Color sample_Li(Surfel& hit, vec3<float> v, vec3<float> *wi){
    		vec3<float> l_ = from - to;
    		l_ = normalize(l_);
    		*wi = l;
    		return l_;
		}
};

#endif