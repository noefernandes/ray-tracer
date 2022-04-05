#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"
#include "vec3.hpp"

class Sphere : public Shape {
	public:

		Sphere(vec3<float> center_, float radius_){
			this-> center = center_;
			this-> radius = radius_;
		}

		virtual ~Sphere(){}

		bool intersect_p(Ray& r){
			float a = dot(r.getDirection(), r.getDirection()); 
			vec3<float> oc = r.getOrigin() - center;
			float b = dot(oc, r.getDirection());
			b = 2.0 * b;
			float c = dot(oc, oc) - (radius*radius);

			float delta = (b*b) - 4*a*c;

			if(delta >= 0)
				return true;

			return false;
		}

		bool intersect(Ray& r, float * t_hit, Surfel *sf){
			Ray ray(r.o, r.d);

			float a = dot(ray.getDirection(), ray.getDirection()); 
			vec3<float> oc = ray.getOrigin() - center;
			float b = dot(oc, ray.getDirection());
			b = 2.0 * b;
			float c = dot(oc, oc) - (radius*radius);

			float delta = b*b - 4*a*c;

			if(delta >= 0){
		        float tmin = (-b - sqrt(delta))/ (2*a);
		        float tmax = (-b + sqrt(delta))/ (2*a);
		        if(tmin < tmax){
		            *t_hit = tmin;
		            sf->p = ray(tmin);
		            sf->n = sf->p - center;
		        }else{
		            *t_hit = tmax;
		            sf->p = ray(tmax);
		            sf->n = sf->p - center;
		        }
		        return true;
		    }

		    return false;
		}

		vec3<float> center;
		float radius;
};

#endif