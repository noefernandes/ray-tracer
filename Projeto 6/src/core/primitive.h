#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <memory>
#include "ray.h"
#include "surfel.h"
#include "material.h"

//using Bounds3f = vec3<float>;

class Material;
class Surfel;

class Primitive {
	public:
		virtual ~Primitive() = default;
		Primitive() = default;
		//Bounds3f world_bounds();
		virtual std::shared_ptr<Material> get_material(){ return material; }
		virtual bool intersect(const Ray& r, Surfel *sf){return false;}
		virtual bool intersect_p(const Ray& r){return false;}
	//private:
		//Material* material;
		std::shared_ptr<Material> material;
};

#endif