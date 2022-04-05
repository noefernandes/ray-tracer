#ifndef PRIMLIST_H
#define PRIMLIST_H

#include "aggregate_primitive.h"
#include <vector>

class PrimList : public AggregatePrimitive{
	public:
		vector<Primitive*> primitives;
		PrimList(std::vector<Primitive*> prim){
			this->primitives = prim;
		}
		PrimList(){};

		//Bounds3f world_bound();
		bool intersect(const Ray& r, Surfel *sf){ return false; }
		bool intersect_p(const Ray& r){ return false; }
};

#endif