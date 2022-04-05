#ifndef CAMERA_H
#define CAMERA_H

#include "rt3.h"
#include "rt3-base.h"


namespace rt3 {
	class Camera{
		public:

            std::string camera_type;

            Camera(std::string camera_type, std::vector<float> screen, vec3<float> lookat, 
            	vec3<float> lookfrom, vec3<float> up){}
            Camera(std::string camera_type, float fovy, vec3<float> lookat, vec3<float> lookfrom, 
            	vec3<float> up) {}

			virtual Ray generate_ray(int x, int y){ 
				vec3<float> v1(0,0,0);
				vec3<float> v2(0,0,0);
				return Ray(v1, v2); 
			}
			virtual Ray generate_ray(float x, float y){ 
				vec3<float> v1(0,0,0);
				vec3<float> v2(0,0,0);
				return Ray(v1, v2); 
			}
			virtual ~Camera(){};
			void setHeightWidth(const int h, const int w){
			    height = h;
			    width = w;

			    if(fovy > 0){
			        float aspect = (float) width/ (float) height;
			        float h = tan(((fovy * M_PI) / 180.0) / 2.0);
			        
			        this->l = -aspect * h;
			        this->r = aspect * h;
			        this->t = h;
			        this->b = -h;
			    }
			}

			float l;
			float r;
			float t;
			float b;

			float fovy;

			vec3<float> gaze;
    		vec3<float> w;
    		vec3<float> u;
    		vec3<float> v;
    		vec3<float> e;

		    float u_esc;
		    float v_esc;

		    int height;
		    int width;

	};

	class PerspectiveCamera : public Camera {
		public:
			PerspectiveCamera(float fovy, vec3<float> lookat, vec3<float> lookfrom, 
				vec3<float> up) : Camera("perspective", fovy, lookat, lookfrom, up)
				{
					//Atribuindo a metade do angulo
					this->fovy = fovy;
			        
			        //calculando o frame
			        this->gaze = lookat - lookfrom;
			    	this->w = normalize(gaze);
			    	this->u = normalize(cross(up, w));
			    	this->v = normalize(cross(w, u));
			    	this->e = lookfrom;
				}

			virtual ~PerspectiveCamera(){};

			Ray generate_ray(int x, int y){
	            u_esc = l + (r - l) * ((y + 0.5) / (float) width);
	            v_esc = b + (t - b) * ((x + 0.5) / (float) height);

	            //w + u * (float) u_esc + v * (float) v_esc
	            vec3<float> temp1 = v * (float) v_esc;
	   			vec3<float> temp2 = u * (float) u_esc;
	   			vec3<float> temp3 = w;
	   			temp3 += temp1;
	   			temp3 += temp2;

	   			//std::cout << u_esc << " " << v_esc << "\n";

	            Ray r(e, temp3);
	            return r;
	        }

	        Ray generate_ray(float x, float y){
	            u_esc = l + (r - l) * ((y + 0.5) / (float) width);
	            v_esc = b + (t - b) * ((x + 0.5) / (float) height);

	            //w + u * (float) u_esc + v * (float) v_esc
	            vec3<float> temp1 = v * (float) v_esc;
	   			vec3<float> temp2 = u * (float) u_esc;
	   			vec3<float> temp3 = w;
	   			temp3 += temp1;
	   			temp3 += temp2;

	            Ray r(e, temp3);
	            return r;
	        }
	};

	class OrthographicCamera : public Camera {
		public:
			
			OrthographicCamera(std::vector<float> screen_window, vec3<float> lookat, vec3<float> lookfrom, 
			vec3<float> up) : Camera("orthographic", screen_window, lookat, lookfrom, up)
			{
				//iniciando screen
		        this->l = screen_window[0];
		        this->r = screen_window[1];
		        this->b = screen_window[2];
		        this->t = screen_window[3];
		        
		        //calculando o frame
		        this->gaze = lookat - lookfrom;
		    	this->w = normalize(gaze);
		    	this->u = normalize(cross(up, w));
		    	this->v = normalize(cross(w, u));
		    	this->e = lookfrom;
			}
			virtual ~OrthographicCamera(){};

			Ray generate_ray(int x, int y){
        		u_esc = l + (r - l) * ( (x + 0.5) / (float) width);
            	v_esc = b + (t - b) * ( (y + 0.5) / (float) height);
            	
            	//e + u * (float) u_esc + v * (float) v_esc
   				vec3<float> temp1 = v * (float) v_esc;
   				vec3<float> temp2 = u * (float) u_esc;
   				vec3<float> temp3 = e;
   				temp3 += temp1;
   				temp3 += temp2; 

   				//std::cout << u_esc << " " << v_esc << "\n";

            	Ray r(temp3, w);
            	return r;
        	}
        	
        	Ray generate_ray(float x, float y){
        		u_esc = l + (r - l) * ( (x + 0.5) / (float) width);
            	v_esc = b + (t - b) * ( (y + 0.5) / (float) height);
            	
            	//e + u * (float) u_esc + v * (float) v_esc
   				vec3<float> temp1 = v * (float) v_esc;
   				vec3<float> temp2 = u * (float) u_esc;
   				vec3<float> temp3 = e;
   				temp3 += temp1;
   				temp3 += temp2; 

            	Ray r(temp3, w);
            	return r;
        	}
	};

	PerspectiveCamera* create_perspective_camera(const ParamSet &ps_camera,  const ParamSet ps_lookat, 
		const Film* film);
	OrthographicCamera* create_orthographic_camera(const ParamSet &ps_camera, const ParamSet ps_lookat, 
		const Film* film);

}

#endif