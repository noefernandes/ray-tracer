#ifndef AGGREGATE_PRIMITIVE_H
#define AGGREGATE_PRIMITIVE_H

#include "material.h"

class AggregatePrimitive : public Primitive{
	public:
		//Deve retornar erro!
		Material* get_material(){ return null; }
		bool intersect(const Ray& r, Surfel *sf) = 0;
		bool intersect_p(const Ray& r) = 0;
};

#endif