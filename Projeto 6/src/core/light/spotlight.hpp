#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "../scene.h"
#include "light.hpp"
#include "../vec3.hpp"
#include <cmath>

class SpotLight : public Light {
	public:
		vec3<float> i;
		vec3<float> scale;
		vec3<float> to;
		float cutoff;
		float falloff;

		SpotLight(std::string type, vec3<float> i, vec3<float> scale, 
		vec3<float> from, vec3<float> to, float cutoff, float falloff) : Light(type, from){
			this->type = type;
			this->i = i;
			this->scale = scale;
			this->from = from;
			this->to = to;
			this->cutoff = cos((cutoff*M_PI)/180.0);
			this->falloff = cos((falloff*M_PI)/180.0);
		}

		void preprocess(Scene& scene){}

		Color sample_Li(Surfel& hit, vec3<float> v, vec3<float>* wi)
		{
			vec3<float> light_dir = normalize(from - hit.p);
			vec3<float> spot_dir = to - from;
			spot_dir.e[0] *= -1;
			spot_dir.e[1] *= -1;
			spot_dir.e[2] *= -1;
			spot_dir = normalize(spot_dir);

			float theta = dot(light_dir, spot_dir);
			
			*wi = i * Falloff(theta);
			return light_dir;
		} 

		float Falloff(float theta){
	        float cos_theta = theta;

	        if(cos_theta < cutoff)
	            return 0.0;
	        if(cos_theta > falloff)
	            return 1.0;

	        float delta = (cos_theta - cutoff) / (falloff - cutoff);
	        return (delta * delta) * (delta * delta);
    	}
};

#endif