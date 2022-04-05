#ifndef GEOMETRIC_PRIMITIVE_H
#define GEOMETRIC_PRIMITIVE_H

#include "shape.h"
#include "material.h"
#include "ray.h"
#include "primitive.h"

class GeometricPrimitive : public Primitive{
	public:
		
		GeometricPrimitive(): Primitive(){}
		
		~GeometricPrimitive() = default;

		//precisa fazer este!
		bool intersect(Ray& r, Surfel* s)
		{
			float thit;
		    if(shape->intersect(r, &thit, s)){
		        if(thit > 0.0 and thit < r.tmax){
		            r.tmax = thit;
		            s->primitive = this;
		            return true;
		        }
		    }
		    
		    return false;
		}

		bool intersect_p(Ray& r)
		{
			if(shape->intersect_p(r)){
            	return true;
		    }
		    return false;
		}

		std::shared_ptr<Material> get_material(){ return material; }
		void set_material(std::shared_ptr<Material> material){ this->material = material; }
		std::shared_ptr<Shape> get_shape(){ return shape; }
		void set_shape(std::shared_ptr<Shape> shape){ this->shape = shape; }
		

		std::shared_ptr<Shape> shape;
};

#endif