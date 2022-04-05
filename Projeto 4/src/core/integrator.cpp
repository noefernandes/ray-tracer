#include "integrator.h"

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

        std::cout << "*********************************************************\n";

        for ( int y = 0 ; y < h ; y++ ) {
            for( int x = 0 ; x < w ; x++ ) {
                // Get the background color in case we hit nothing.
                Color color = scene->getBackground()->sampleXYZ( Point2f{float(x)/float(w), float(y)/float(h)} );
                // Determine the ray for the current camera type.
        
                Ray ray = camera->generate_ray(x, y); // Generate the ray from (x,y)
                // Determine the incoming light.
                Color L =  Li( ray, scene, color );
                //std::cout << L.e[0] << " " << L.e[1] << " " << L.e[2] << "\n";
                // Add color (radiance) to the image.
                camera->film->add_sample( Point2f( x, h-y ), L ); // Set color of pixel (x,y) to L.
            }
        }
        // Send image color buffer to the output file.
        camera->film->write_image();
    }

    Color FlatIntegrator::Li(Ray& ray, std::shared_ptr<Scene> scene, Color bkg_color )
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
}