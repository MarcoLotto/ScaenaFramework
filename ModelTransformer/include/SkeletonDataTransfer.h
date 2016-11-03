/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "glm/glm.hpp"
#include "InformationDataTransfer.h"
#include "KeyFrameBruteDataTransfer.h"
#include <string>
#include <list>
#include <vector>
#include "KeyFrameBruteDataTransfer.h"
using namespace std;

typedef glm::vec3 vec3;
typedef glm::vec2 vec2;
typedef glm::mat4 mat4;

class SkeletonDataTransfer : public InformationDataTransfer{

private:
	list<SkeletonDataTransfer*>* childJoint;
	vector<KeyFrameBruteDataTransfer*>* keyFrames;
	bool bindedToMesh;

public:
	SkeletonDataTransfer(){ this->childJoint = new list<SkeletonDataTransfer*>(); this->jointName = ""; this->jointIndex = -1; this->bindedToMesh = false; 
							this->keyFrames = new vector<KeyFrameBruteDataTransfer*>();}
	virtual ~SkeletonDataTransfer(){ delete this->childJoint; delete this->keyFrames; }

	string jointName;
	int jointIndex;	

	void addChild(SkeletonDataTransfer* mdt){ this->childJoint->push_back(mdt); } 
	list<SkeletonDataTransfer*>* getChilds(){ return this->childJoint; }

	void addKeyFrame(KeyFrameBruteDataTransfer* kf){ this->keyFrames->push_back(kf); } 
	vector<KeyFrameBruteDataTransfer*>* getKeyFrames(){ return this->keyFrames; }

	// Identifica si el joint esta bindeado con algun mesh
	void setBindedToMesh(bool value){ this->bindedToMesh = value; }
	bool isBindedToMesh(){ return this->bindedToMesh; }
};