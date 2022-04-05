#ifndef _POINT3_VALUE
#define _POINT3_VALUE
#include <iostream>

#include <string>
#include <cmath>
#include <sstream>
#include <vector>


class point3{
    public:
        float x;
        float y;
        float z;

        point3(){ x = 0.0; y = 0.0; z = 0.0;};
        point3(float x, float y, float z){ this->x = x; this->y = y; this->z = z;};

        point3(float coordinate[]){
        	this->x = coordinate[0];
        	this->y = coordinate[1];
        	this->z = coordinate[2];
    	}

        point3 operator-(const point3 &p)
        {
            float x1 = x - p.x;
            float y1 = y - p.y;
            float z1 = z - p.z;

            return point3(x1, y1, z1);
        }

        point3 operator+(const point3 &p)
        {
            float x1 = x + p.x;
            float y1 = y + p.y;
            float z1 = z + p.z;

            return point3(x1, y1, z1);
        }

};


inline float distance( const point3& p1, const point3& p2 ){
	float x = p1.x - p2.x;
	float y = p1.y - p2.y;
	float z = p1.z - p2.z;

	return sqrt(x*x + y*y + z*z);
}

inline float distanceSqrt( const point3& p1, const point3& p2 ){
	float x = p1.x - p2.x;
	float y = p1.y - p2.y;
	float z = p1.z - p2.z;

	return (x*x + y*y + z*z);
}

inline istream& operator>>(istream &is, point3 &t){
    is >> t.x >> t.y >> t.z;
    return is;
}


inline ostream& operator<<(ostream &os, const point3 &t){
    os << t.x << " " << t.y << " " << t.z << "\n";
    return os;
}

inline bool operator!=(const point3 &p1, const point3 &p2) {
    if ((p1.x != p2.x) || (p1.y != p2.y) 
    	|| (p1.z != p2.z) ){
            return true;
        }

    return false;
}

inline bool operator==(const point3 &p1, const point3 &p2) {
    if ((p1.x != p2.x) || (p1.y != p2.y) 
    	|| (p1.z != p2.z) ){
            return false;
        }

        return true;
}




#endif