#ifndef _SCENE_H_
#define _SCENE_H_

#include "background.h"
#include "geometric_primitive.h"
#include  <vector>
#include "light/light.hpp"
#include "light/ambientlight.hpp"

namespace rt3{

class Scene{
        
    public:
        Scene(std::shared_ptr<Background> background, 
            std::vector<std::shared_ptr<GeometricPrimitive>> objList,
            std::vector<std::shared_ptr<Light>> lights,
            AmbientLight* ambient)
        {
            this->background = background;
            this->objList = objList;
            this->lights = lights;
            this->ambient = ambient;
        }

        ~Scene() = default;

        void setBackground(std::shared_ptr<Background> background)
        {
            this->background = background;
        }
        
        void setObjList(std::vector<std::shared_ptr<GeometricPrimitive>> objList)
        {
            this->objList = objList;
        }

        void setLights(std::vector<std::shared_ptr<Light>> lights)
        {
            this->lights = lights;
        }

        std::shared_ptr<Background> getBackground(){return this->background;}
        std::vector<std::shared_ptr<GeometricPrimitive>> getObjList(){return this->objList;}
        std::vector<std::shared_ptr<Light>> getLights(){return this->lights;}

        std::shared_ptr<Background> background;
        std::vector<std::shared_ptr<GeometricPrimitive>> objList;
        std::vector<std::shared_ptr<Light>> lights;
        AmbientLight * ambient = nullptr;
};

}

#endif