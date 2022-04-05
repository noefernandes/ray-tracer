#include "camera.h"

namespace rt3 {



	PerspectiveCamera* create_perspective_camera(const ParamSet &ps_camera,  const ParamSet ps_lookat, const Film* film){
		//inicializando o fovy
        float fovy = rt3::retrieve<float>(ps_camera, "fovy");
      	//Recuperando lookat, lookfrom e up
      	vec3<float> lookat = rt3::retrieve<vec3<float>>(ps_lookat, "look_at");
      	vec3<float> lookfrom = rt3::retrieve<vec3<float>>(ps_lookat, "look_from");
      	vec3<float> up = rt3::retrieve<vec3<float>>(ps_lookat, "up");
      	return new PerspectiveCamera(fovy, lookat, lookfrom, up);
	}

	OrthographicCamera* create_orthographic_camera(const ParamSet &ps_camera,  const ParamSet ps_lookat, const Film* film){
		//inicializando a screen_window
        std::vector<float> screen_window = rt3::retrieve<std::vector<float>>(ps_camera, "screen_window");
      	//Recuperando lookat, lookfrom e up
      	vec3<float> lookat = rt3::retrieve<vec3<float>>(ps_lookat, "look_at");
      	vec3<float> lookfrom = rt3::retrieve<vec3<float>>(ps_lookat, "look_from");
      	vec3<float> up = rt3::retrieve<vec3<float>>(ps_lookat, "up");
      	return new OrthographicCamera(screen_window, lookat, lookfrom, up);
	}
}