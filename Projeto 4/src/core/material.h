#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "surfel.h"

using Color = vec3<float>;

class Material {
	public:
		virtual void scatter(Surfel* isect, Ray* r) = 0;
		virtual Color kd() = 0;

		//estava no flat
		Color color;
};

#endif