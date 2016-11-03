/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "MeshSkeletonInterpolator.h"

MeshSkeletonInterpolator::MeshSkeletonInterpolator(){
	this->initialPose = NULL;
	this->endPose = NULL;
	this->actualPose = new MeshSkeleton();
	this->timeInterpolator = new TimeParameterInterpolator(0.0f, 1.0f, 1000);
	this->timeInterpolator->setInverseAnimationEnabled(false);
	this->timeInterpolator->setContiniumAnimationEnabled(false);
}
MeshSkeletonInterpolator::~MeshSkeletonInterpolator(){
	delete this->actualPose;
	delete this->timeInterpolator;
}

void MeshSkeletonInterpolator::update(){
	if(this->initialPose != NULL && this->endPose != NULL){
		this->initialPose->interpolate(this->actualPose, this->endPose, this->timeInterpolator->getCurrentValue());
	}
}

void MeshSkeletonInterpolator::setInitialPose(MeshSkeleton* skeleton){ 
	this->initialPose = skeleton; 
	if(this->actualPose != NULL){
		delete this->actualPose;
	}
	if(this->initialPose != NULL)
		this->actualPose = this->initialPose->clone();
	else
		this->actualPose = new MeshSkeleton();
}