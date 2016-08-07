/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "SkeletonDataTransfer.h"

using namespace std;

typedef glm::vec3 vec3;
typedef glm::vec2 vec2;

class KeyFrameBruteDataTransfer{

public:
	float animationTime;

	vec3 position;
	vec3 rotation;
	vec3 scale;	
	
};