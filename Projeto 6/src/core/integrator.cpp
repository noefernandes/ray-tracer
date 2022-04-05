#include "integrator.h"
#include "ray.h"

namespace rt3{

	FlatIntegrator* create_flat_integrator( const ParamSet &ps, std::shared_ptr<Camera> camera)
	{
		return new FlatIntegrator(camera);
	}

	void FlatIntegrator::render(std::shared_ptr<Scene> scene) {
        // Always call the preprocess() before doing any rendering.
        // This might be just an empty method, or not, depending on the integrator's needs.
        preprocess(scene);
        
        // This might just be a tile (part) of an image, rendered in parallel.
        auto res  = camera->film->get_resolution();
        int w = res[0];
        int h = res[1];

        //std::cout << "*********************************************************\n";

        for ( int y = 0 ; y < h ; y++ ) {
            for( int x = 0 ; x < w ; x++ ) {
                // Get the background color in case we hit nothing.
                Color color = scene->getBackground()->sampleXYZ( Point2f{float(x)/float(w), float(y)/float(h)} );
                // Determine the ray for the current camera type.
        
                Ray ray = camera->generate_ray(x, y); // Generate the ray from (x,y)
                // Determine the incoming light.
                Color L =  Li( ray, scene, color, 0 );
                //std::cout << L.e[0] << " " << L.e[1] << " " << L.e[2] << "\n";
                // Add color (radiance) to the image.
                camera->film->add_sample( Point2f( x, h-y ), L ); // Set color of pixel (x,y) to L.
            }
        }
        // Send image color buffer to the output file.
        camera->film->write_image();
    }

    Color FlatIntegrator::Li(Ray& ray, std::shared_ptr<Scene> scene, Color bkg_color, int depth )
    {
        Color color = bkg_color;
        auto objList = scene->getObjList();

        
        Surfel sf;
        for(size_t k = 0; k < objList.size(); k++)
        {
            if(objList[k]->intersect(ray, &sf))
            {
                color = sf.primitive->get_material()->kd();
            }
        }

        return color;
    }

    BlinnPhongIntegrator* create_blinn_integrator( const ParamSet &ps, std::shared_ptr<Camera> camera, int depth)
    {
        return new BlinnPhongIntegrator(camera, depth);
    }

    void BlinnPhongIntegrator::render(std::shared_ptr<Scene> scene) {
        // Always call the preprocess() before doing any rendering.
        // This might be just an empty method, or not, depending on the integrator's needs.
        preprocess(scene);
        

        // This might just be a tile (part) of an image, rendered in parallel.
        auto res  = camera->film->get_resolution();
        int w = res[0];
        int h = res[1];

        //std::cout << "*********************************************************\n";

        for ( int y = 0 ; y < h ; y++ ) {
            for( int x = 0 ; x < w ; x++ ) {
                // Get the background color in case we hit nothing.
                Color color = scene->getBackground()->sampleXYZ( Point2f{float(x)/float(w), float(y)/float(h)} );
                // Determine the ray for the current camera type.
        
                Ray ray = camera->generate_ray(x, y); // Generate the ray from (x,y)
                // Determine the incoming light.
                //std::cout << color.e[0] << " " << color.e[1] << " " << color.e[2] << "\n";
                Color L =  Li( ray, scene, color/255, 0 );
                //std::cout << "AQUI +++++++++++++++++++++++++++++++++++++++++=\n";
                //std::cout << L.e[0] << " " << L.e[1] << " " << L.e[2] << "\n";
                // Add color (radiance) to the image.
                camera->film->add_sample( Point2f( x, h-y ), L ); // Set color of pixel (x,y) to L.
            }
        }

        // Send image color buffer to the output file.
        camera->film->write_image();
    }

    Color BlinnPhongIntegrator::Li(Ray& ray, std::shared_ptr<Scene> scene, Color bkg_color, int depth )
    {
        Color color = bkg_color;
        auto obj_list = scene->objList;
        auto lights_ = scene->lights;
        Surfel sf;

        for(size_t i = 0; i < obj_list.size(); i++)
        {
            if(obj_list[i]->intersect(ray, &sf))
            {
                BlinnMaterial *blinn = dynamic_cast<BlinnMaterial *>(sf.primitive->get_material().get());
                vec3<float> n = normalize(sf.n);
                vec3<float> c;
                vec3<float> wi;

                //Calculando contribuição de cada luz
                for(size_t j = 0; j < lights_.size(); j++)
                {  
                    //Calculando o shadow ray
                    vec3<float> l = lights_[j]->sample_Li(sf, ray.getOrigin(), &wi);
                    Ray shadow_ray;
                    if (lights_[j]->type == "directional")
                    {
                        shadow_ray.o = sf.p;
                        shadow_ray.d = l;
                    }
                    else
                    {
                        //Para os diferentes de directional precisa
                        //encontrar a distância
                        float distance = dist(sf.p, lights_[j]->from);
                        shadow_ray.o = sf.p;
                        shadow_ray.d = l;
                        shadow_ray.tmin = 0.0;
                        shadow_ray.tmax = distance;
                    }
                    
                    //Testando se o shadow ray bateu em algum objeto
                    bool hit = false;
                    for(size_t k = 0; k < obj_list.size(); k++)
                    {
                        //Testando se o objeto testado é diferente do atual
                        if(k != i)
                        {
                            hit = obj_list[k]->intersect_p(shadow_ray);
                        }

                        if(hit)
                        {
                            break;
                        }
                    }

                    if(!hit)
                    {
                        vec3<float> v = ray.getOrigin() - sf.p;
                        v = normalize(v);

                        //Encontrando o ângulo entre a normal e o bissetor
                        vec3<float> h = (v + l) / length(v + l);

                        //calculando a diffuse contribution
                        c = c + (blinn->kd() * wi * std::max(0.f, dot(n, l)));
                        //calculando a specular contribution
                        c = c + (blinn->ks() * wi * std::pow(std::max(0.f, dot(n, h)), blinn->glossiness));
                    }

                }

                //Adicionando contribuição da luz ambiente
                if (scene->ambient != nullptr)
                {
                    vec3<float> l_ = scene->ambient->l;
                    color = c + (blinn->ka() * l_);
                }
                else
                {
                    color = c;
                }

                if(depth < max_depth)
                {
                    vec3<float> dir = ray.getDirection() - (n*(2*(dot(ray.getDirection(), n))));
                    Ray reflected_ray = Ray(sf.p, dir);
                    color = color + blinn->km() * Li(reflected_ray, scene, bkg_color, depth + 1);
                }

                if(color.e[0] >= 1.0){
                    //std::cout << color.e[0] << "\n";
                    color.e[0] = 0.9999;
                }

                if(color.e[1] >= 1.0){
                    //std::cout << color.e[1] << "\n";
                    color.e[1] = 0.9999;
                }

                if (color.e[2] >= 1.0){
                    //std::cout << color.e[2] << "\n";
                    color.e[2] = 0.9999;
                }
            }
        }

        return color;
    }

    
}