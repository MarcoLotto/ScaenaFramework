/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshDataTransfer.h"
#include "SkeletonDataTransfer.h"

using namespace std;

typedef glm::vec3 vec3;
typedef glm::vec2 vec2;

class ModelDataTransfer{

private:
	MeshDataTransfer* rootMesh;
	SkeletonDataTransfer* rootSkeleton;


public:
	string filepath;

	ModelDataTransfer(){ this->rootMesh = NULL; this->rootSkeleton = NULL; }
		
	void setRootMesh(MeshDataTransfer* mdt){ this->rootMesh = mdt; } 
	void setRootSkeleton(SkeletonDataTransfer* sdt){ this->rootSkeleton = sdt; } 

	MeshDataTransfer* getRootMesh(){ return this->rootMesh; } 
	SkeletonDataTransfer* getRootSkeleton(){ return this->rootSkeleton; } 	
};