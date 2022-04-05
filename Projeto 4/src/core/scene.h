#ifndef _SCENE_H_
#define _SCENE_H_

#include "background.h"
#include "geometric_primitive.h"
#include  <vector>

namespace rt3{

class Scene{
    private:
        std::shared_ptr<Background> background;
        std::vector<std::shared_ptr<GeometricPrimitive>> objList;
    public:
        Scene(std::shared_ptr<Background> background, std::vector<std::shared_ptr<GeometricPrimitive>> objList)
        {
            this->background = background;
            this->objList = objList;
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

        std::shared_ptr<Background> getBackground(){return this->background;}
        std::vector<std::shared_ptr<GeometricPrimitive>> getObjList(){return this->objList;}

        
};

}

#endif