#ifndef _SCENE_H_
#define _SCENE_H_

#include "background.h"
#include "camera.h"
#include "primitive.h"
#include  <vector>

namespace rt3{

    class Scene{
    private:
        Camera *camera;
        Background *background;
        std::vector<Primitive> objectsList;
    public:
        Scene(){};
        ~Scene();

        Scene(Camera *camera, Background *background, std::vector<Primitive> objList);
        void setCamera(Camera *camera);
        void setBackground(Background *background);
        void setObjList(std::vector<Primitive> * objList);

        Camera* getCamera();
        Background* getBackground();
        std::vector<Primitive> getObjList();

    };

    Scene(Camera *camera, Background *background, std::vector<Primitive> objList);
    {
        this->camera = camera;
        this->background = background;
        this->objectsList = objList;
    }

    Scene::~scene()
    {
        if(this->camera){
            delete camera;
        }

        if(this->background){
            delete background;
        }
    }

    Scene::setBackground(Background * background){
        this->background = bg;
    }
    Scene::void setObjList(std::vector<Primitive> * objList){
        this->objectsList = objList;
    }
    Scene::Camera* setCamera(Camera *camera){
        this->camera = camera;
    }

  
    Scene::Camera* getCamera(){return this->camera;}
    Scene::Background* getBackground(){return this->background;}
    Scene::std::vector<Primitive> getObjList(){return this->objectsList;}
}


#endif