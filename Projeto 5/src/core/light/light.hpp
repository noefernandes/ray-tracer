#ifndef LIGHT_H
#define LIGHT_H

#include "../scene.h"
#include "../vec3.hpp"
#include "../surfel.h"

using Color = vec3<float>;

class Scene;

class Light {
	public:
		~Light(){};
		Light(std::string type, vec3<float> from){this->type = type;}
		virtual Color sample_Li(Surfel& hit, vec3<float> v, vec3<float>* wi) = 0;
		virtual void preprocess(Scene& scene) = 0;

		std::string type;
		vec3<float> from; //origem da luz 
};

#endif