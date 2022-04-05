#ifndef SHAPE_
#define SHAPE_

#include "material.h"
#include "vec3.hpp"

class Shape {
	public:
		//bool flip_normals;

		//Shape(bool flip_n);
	    //Bounds3f world_bounds();

		//Shape(vec3<float> center, float radius){}

	    virtual bool intersect(Ray& r, float * t_hit, Surfel *sf) = 0;
		virtual bool intersect_p(Ray& r) = 0;
};

#endif