/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

#include "ControlPointSkinJointData.h"
#include "glm\glm.hpp"
#include <vector>
using namespace std;
typedef glm::vec3 vec3;

class ControlPoint{

public:
	vector<ControlPointSkinJointData*>* skinInfo;
	vec3 vertex;

	ControlPoint(vec3 vertex){ this->vertex = vertex; this->skinInfo = new vector<ControlPointSkinJointData*>(); }

	~ControlPoint(){
		for(unsigned int i=0; this->skinInfo->size(); i++){
			delete this->skinInfo->at(i);
		}
		delete this->skinInfo;
	}
};