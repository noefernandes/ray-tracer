#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "rt3.h"
#include "surfel.h"
#include "material.h"

class Primitive {
	public:
		virtual ~Primitive(){};
		Primitive() = default;
		virtual bool intersect(const Ray& r, Surfel *sf){return false;}
		virtual bool intersect_p(const Ray& r){return false;}
	//private:
		//std::shared_ptr<Material> material;
};

#endif