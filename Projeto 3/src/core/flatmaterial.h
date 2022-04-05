#ifndef FLATMATERIAL_H
#define FLATMATERIAL_H

#include "material.h"
#include "rt3.h"

using Color = vec3<float>;

class FlatMaterial : public Material{
	public:
		FlatMaterial(Color c){
			color = c;
			type = "flat";
		}

		~FlatMaterial(){}

		Color getColor(){ return color; }

	private:
		Color color;
		std::string type;
};

#endif