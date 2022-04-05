#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "../scene.h"
#include "light.hpp"
#include "../vec3.hpp"


class PointLight : public Light {
	public:

		PointLight(std::string type, vec3<float> i, vec3<float> scale, vec3<float> from) : Light(type, from){
			this->type = type;
			this->i = i;
			this->scale = scale;
			this->from = from;
		}

		void preprocess(Scene& scene){}

		Color sample_Li(Surfel& hit, vec3<float> v, vec3<float>* wi){
			vec3<float> l = normalize(from - hit.p);
			*wi = i;
			return l;
		} 

		vec3<float> i;		//intensidade
		vec3<float> scale;  //escala
};

#endif