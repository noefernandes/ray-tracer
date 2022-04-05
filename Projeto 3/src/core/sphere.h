#ifndef SPHERE_H
#define SPHERE_H

#include "primitive.h"
#include "vec3.hpp"

class Sphere : public Primitive {
	public:

		Sphere(vec3<float> center, float radius){
			this->radius = radius;
			this->center = center;
		}

		virtual ~Sphere(){}

		bool intersect(const Ray& r, Surfel *sf);
		bool intersect_p(const Ray& r);

	public:
		float radius;
		vec3<float> center;
};

bool Sphere::intersect_p(const Ray& r){
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

bool Sphere::intersect(const Ray& r, Surfel *sf){
	float a = dot(r.getDirection(), r.getDirection()); 
	vec3<float> oc = r.getOrigin() - center;
	float b = dot(oc, r.getDirection());
	b = 2.0 * b;
	float c = dot(oc, oc) - (radius*radius);

	float delta = b*b - 4*a*c;

	if(delta >= 0)
		return true;

	return false;
}

#endif