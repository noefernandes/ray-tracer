#ifndef SURFEL_H
#define SURFEL_H

#include "rt3.h"
#include "primitive.h"

using Point = vec3<float>;
using Vector = vec3<float>;
using Point2f = point2<float>;

class Primitive;

class Surfel {
	public:
		Surfel( const Point&p,const Vector&n, const Vector&wo, float time, 
			const Point2f& uv, Primitive *pri )
			: p{p}, n{n}, wo{wo}, time{time}, uv{uv}, primitive{pri}
            {/* empty */};

        Surfel(){}

		Point p;        //!< Contact point.
		Vector n;       //!< The surface normal.
		Vector wo;      //!< Outgoing direction of light, which is -ray.
		float time;     //!< Time of contact.
		Point2f uv;     //!< Parametric coordinate (u,v) of the hit surface.
		Primitive *primitive=nullptr; //!< Pointer to the primitive.
};

#endif