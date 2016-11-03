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

class KeyFrameDataTransfer{

private:
	SkeletonDataTransfer* skeletonRoot;

public:
	float animationTime;

	void setRootSkeleton(SkeletonDataTransfer* skeleton){ this->skeletonRoot = skeleton; }
	SkeletonDataTransfer* getRootSkeleton(){ return this->skeletonRoot; }
	
};