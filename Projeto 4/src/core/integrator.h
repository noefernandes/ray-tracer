#ifndef _INTEGRATOR_H_
#define _INTEGRATOR_H_

#include "rt3.h"
#include "scene.h"
#include "vec3.hpp"
#include "ray.h"
#include "camera.h"
#include <memory>

using Color = vec3<float>;

namespace rt3 {

class Integrator {
	public:
		virtual ~Integrator() = default;
		virtual void render( std::shared_ptr<Scene> scene ){}
        virtual Color Li( Ray& ray, std::shared_ptr<Scene> scene, Color bkg_color ){ return Color(0,0,0); }
        virtual void preprocess(std::shared_ptr<Scene> scene ){}
};

class FlatIntegrator : public Integrator {
	//=== Public interface
	public:
	   ~FlatIntegrator() = default;
		FlatIntegrator( std::shared_ptr<Camera> cam ) : camera{cam}{}
		Color Li( Ray& ray, std::shared_ptr<Scene> scene, Color bkg_color );
		void preprocess(std::shared_ptr<Scene> scene ){}
        void render(std::shared_ptr<Scene> scene);

        std::shared_ptr<Camera> camera;		
};

FlatIntegrator* create_flat_integrator( const ParamSet &ps, std::shared_ptr<Camera> camera );

}

#endif

 