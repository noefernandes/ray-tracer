#ifndef FLATMATERIAL_H
#define FLATMATERIAL_H

#include "material.h"
#include "rt3.h"

//using Color = vec3<float>;

class FlatMaterial : public Material{
	public:
		FlatMaterial(Color c){
			color = c;
		}

		~FlatMaterial(){}

		void scatter(Surfel* isect, Ray *r){}
		Color kd(){return color;}
};

#endif