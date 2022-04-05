#ifndef FLATINTEGRATOR_H
#define FLATINTEGRATOR_H


#include "integrator.h"
#include "scene"

using Color24 = vec3;

class FlatIntegrator: public Integrator{
    public :
    Color24 Li(const Ray& ray, const Scene& scene, Color24 bkg_color );

    ~FlatIntegrator(){};


}
Color24 FlatIntegrator::Li(const Ray& ray, const Scene& scene, Color24 bkg_color ){
    Color24 L = bkg_color;
    Surfel isect; 
    auto objList = scene->objList;

    for(int i = 0; i < objList.size(); i++){
        if(objList[i]->intersect(ray,isect)){
             // Some form of determining the incoming radiance at the ray's origin.
            // For this integrator, it might just be:
            // Polymorphism in action.
            FlatMaterial *fm = dynamic_cast< FlatMaterial *>( iscet.primitive->get_material() );
            // Assign diffuse color to L.
            L = fm->kd(); // Call a method present only in FlatMaterial.
    
        }else{
            L = Color24(0,0,0);
        }
    }
       
    return L;

}


#endif