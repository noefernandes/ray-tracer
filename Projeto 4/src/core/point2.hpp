#ifndef _POINT2_VALUE
#define _POINT2_VALUE
#include <iostream>

#include <string>
#include <cmath>
#include <sstream>
#include <vector>

using namespace std;
template<typename T>
class point2{
    public:
        T coordinate[2];// coordinate[0] = 0 and coordinate[1] = 1

        point2<T>(){ coordinate[0] = 0.0; coordinate[1] = 0.0;};
        point2<T>(T x, T y){ this->coordinate[0] = x; this->coordinate[1] = y;};

        point2<T>(T coordinate[]){
        	this->coordinate[0] = coordinate[0];
        	this->coordinate[1] = coordinate[1];
    	}

        inline T distance( const point2<T>& p1){
            T x = coordinate[0] - p1.coordinate[0];
            T y = coordinate[1] - p1.coordinate[1];

            return sqrt(x*x + y*y );
        }

        inline T& operator[](int i) {return coordinate[i];}
        inline T operator[](int i) const {return coordinate[i];};

        inline T distanceSqrt( const point2<T>& p1 ){
            T x = coordinate[0] - p1.coordinate[0];
            T y = coordinate[1] - p1.coordinate[1];

            return (x*x + y*y );
        }

        inline istream& operator>>(istream &is){
            is >> coordinate[0] >> coordinate[1] ;
            return is;
        }


        inline ostream& operator<<(ostream &os){
            os << coordinate[0] << " " << coordinate[1] << " " << "\n";
            return os;
        }

        inline bool operator!=(const point2<T> &p1) const{
            if ((coordinate[0] != p1.coordinate[0]) || (coordinate[1] != p1.coordinate[1])  ){
                    return true;
                }

            return false;
        }

        inline bool operator==(const point2<T> &p1) const{
            if ((coordinate[0] != p1.coordinate[0]) || (coordinate[1] != p1.coordinate[1])){
                    return false;
                }

                return true;
        }


};




#endif