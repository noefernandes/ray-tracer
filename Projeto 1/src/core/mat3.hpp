#ifndef _MAT3_VALUE_
#define _MAT3_VALUE_

#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>
#include "vec3.h"

using namespace std;

class mat3
{
public:
	vector<vec3> mat;
	int size;

	mat3(){
		mat.push_back(vec3());
	};
	mat3(vector<vec3> mat){
	 	this->mat.resize(mat.size());
	 	for (int i = 0; i < mat.size(); ++i)
	 	{
	 		this->mat[i] = mat[i];
	 	}
	}

	~mat3();
	
};


#endif