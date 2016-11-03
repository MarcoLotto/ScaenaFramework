/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "KeyFrameDataTransfer.h"

using namespace std;

typedef glm::vec3 vec3;
typedef glm::vec2 vec2;

class AnimationDataTransfer{

public:
	string name;
	list<KeyFrameDataTransfer*>* keyframes;

	AnimationDataTransfer(){ this->keyframes = new list<KeyFrameDataTransfer*>(); }	
	~AnimationDataTransfer(){ delete this->keyframes; }		
};