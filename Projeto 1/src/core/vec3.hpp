#ifndef _VEC3_VALUE
#define _VEC3_VALUE

#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>

using namespace std;
template<typename T>
class vec3
{

public:
    T e[4]; // e[0] = x, e[1] = y, e[2] = z, e[3] = a.

    vec3()
    {
        e[0] = 0.0;
        e[1] = 0.0;
        e[2] = 0.0;
        e[3] = 0.0;
    }

    vec3(T x, T y, T z)
    {
        e[0] = x;
        e[1] = y;
        e[2] = z;
        e[3] = 0.0;
    }

    vec3(T x, T y, T z, T a)
    {
        e[0] = x;
        e[1] = y;
        e[2] = z;
        e[3] = a;
    }

    T getX(){return e[0];}
    void setX(T x){e[0] = x;}
    
    T getY(){return e[1];}
    void setY(T y){e[1] = y;}

	T getZ(){return e[2];}
    void setZ(T z){e[2] = z;}

    T getA(){return e[3];}
    void setA(T a){e[3] = a;}

	T getR(){return e[0];}
    void setR(T r){e[0] = r;}
    
    T getG(){return e[1];}
    void setG(T g){e[1] = g;}

	T getB(){return e[2];}
    void setB(T b){e[2] = b;}    

    vec3<T>(T vetor[])
    {
        e[0] = vetor[0];
        e[1] = vetor[1];
        e[2] = vetor[2];
        e[3] = vetor[3];
    }

    inline T operator[](int i) const {return e[1];};
    inline T& operator[](int i) {return e[i];};

    inline bool isWhite(){
        if (e[0] == 255 && e[1] == 255 && e[2] == 255 && e[3] == 255){
            return true;
        }

        return false;
    }

    inline bool isBlack(){
        if (e[0] == 0 && e[1] == 0 && e[2] == 0 && e[3] == 0){
            return true;
        }

        return false;
    }

    inline vec3<T> operator=(const vec3<T> &v){
        e[0] = v.e[0];
        e[1] = v.e[1];
        e[2] = v.e[2];

        return *this;
    }

    inline vec3<T> operator*=(const vec3<T> &v){
        e[0] *= v.e[0];
        e[1] *= v.e[1];
        e[2] *= v.e[2];

        return *this;
    }

    inline vec3<T> operator*=(const T t){
        auto k = 1.0 / t;
        e[0] *= k;
        e[1] *= k;
        e[2] *= k;
        return *this;
    }

    inline vec3<T> operator/=(const vec3<T> &v){
        e[0] /= v.e[0];
        e[1] /= v.e[1];
        e[2] /= v.e[2];

        return *this;
    }

    inline vec3<T> operator/=(const T t){
        auto k = 1.0 / t;
        e[0] /= k;
        e[1] /= k;
        e[2] /= k;
        return *this;
    }

    inline void make_unit_vector(){
        auto k = 1.0 / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
        e[0] *= k;
        e[1] *= k;
        e[2] *= k;
    }



    inline std::istream& operator>>(istream &is){
        is >> e[0] >> e[1] >> e[2];
        return is;
    }


    inline std::ostream& operator<<(ostream &os){
        os << e[0]<< " " << e[1] << " " << e[2] << "\n";
        return os;
    }

    inline vec3<T> operator+(const vec3<T> &v1){
        return vec3<T>(e[0] + v1.e[0], e[1] + v1.e[1]
            , e[2] + v1.e[2], e[3] + v1.e[3]);
    }

    inline vec3<T> operator-(const vec3<T> &v1){
        return vec3<T>(e[0] - v1.e[0], e[1] - v1.e[1]
            , e[2] - v1.e[2], e[3] - v1.e[3]);
    }


    inline vec3<T> operator*(const T &k) {
        return vec3<T>(e[0] * k, e[1] * k
            , e[2] * k, e[3] * k);
    }

    //Produto vetorial utilizando a regra da mão esquerda
    inline vec3<T> operator*(const vec3<T> &v1){
        return vec3<T>(e[1] * (v1.e[2] - e[2]) * v1.e[1],
                    (-1) * (e[0] * v1.e[2] - e[2] * v1.e[0]),
                    e[0] * v1.e[1] - e[1] * v1.e[0]);
    }
 	inline T length() const{return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);}
 	inline T squared_length() const{
 		return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
 	};
};
#endif