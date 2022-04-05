#ifndef AMBIENTLIGHT_H
#define AMBIENTLIGHT_H

#include "light.hpp"
#include "../scene.h"
#include "../vec3.hpp"

class Scene;

class AmbientLight : public Light {
	public:

		AmbientLight(std::string type, vec3<float> l) : Light(type, from){
			this->type = type;
			this->l = l;
		}

		void preprocess(Scene& scene){}

		Color sample_Li(Surfel& hit, vec3<float> v, vec3<float>* wi){
			//std::cout << "entra +++++++++++++++++++++++++++++++++=\n";
			return l;
		}

		vec3<float> l;
};

#endif