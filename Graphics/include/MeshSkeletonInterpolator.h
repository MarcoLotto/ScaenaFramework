/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshSkeleton.h"
#include "TimeParameterInterpolator.h"

class MeshSkeletonInterpolator{

private:
	MeshSkeleton* initialPose;
	MeshSkeleton* endPose;
	MeshSkeleton* actualPose;

	TimeParameterInterpolator* timeInterpolator;

public:
	MeshSkeletonInterpolator();
	virtual ~MeshSkeletonInterpolator();

	void update();
	
	MeshSkeleton* getActualPose(){ return this->actualPose; }
	TimeParameterInterpolator* getTimeInterpolator(){ return this->timeInterpolator; }

	void setInitialPose(MeshSkeleton* skeleton);
	void setEndPose(MeshSkeleton* skeleton){ this->endPose = skeleton; }	

	MeshSkeleton* getInitialPose(){ return this->initialPose; }
	MeshSkeleton* getEndPose(){ return this->endPose; }

	void play(){ this->timeInterpolator->setPlay(); }
	void stop(){ this->timeInterpolator->setStop(); }
};