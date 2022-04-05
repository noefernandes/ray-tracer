#ifndef RAY_H
#define RAY_H

#include <iostream>
#include "vec3.hpp"

class Ray {
    public:
        Ray (vec3<float>& o, vec3<float>& d ) : o{o}, d{d} {/*empty*/}

        void normalize(){
            float norm = std::sqrt(d.e[0]*d.e[0] + d.e[1] * d.e[1] + d.e[2] * d.e[2]);
            this->d.e[0] /= norm;
            this->d.e[1] /= norm;
            this->d.e[2] /= norm;
        } 

        vec3<float> operator()(float t) { 
            vec3<float> temp = d * t; 
            return o + temp; 
        }

        void show(){
            std::cout <<  "o: " << o.e[0] << " " << o.e[1] << " " << o.e[2] << "\nd: " << d.e[0] << " " << d.e[1] << " " << d.e[2] << "\n";  
        }
        
        friend std::ostream& operator<<(std::ostream& os, Ray& r);    
    private:
        vec3<float> o; //!< origin point
        vec3<float> d; //!< direction
    // ... the methods goes here
};

#endif