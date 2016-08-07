/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SkinnedAnimation.h"

SkinnedAnimation::SkinnedAnimation(string name){ 
	this->name = name; 
	this->actualKeyIndex = 0; 
	this->hasLoop = false;
	this->actualKeyFrameTime = 0.0f;
	this->smoothKeyFrame = NULL;
	this->smoothKeyFrameExecuted = false;
	this->reverseAnimationActive = false;
	this->inverseAnimationLoopEnabled = false;
}

SkinnedAnimation::~SkinnedAnimation(){ 
	for(unsigned int i=0; i < keyFrames.size(); i++){ 
		delete this->keyFrames[i]; 
	} 
	if(this->smoothKeyFrame != NULL){
		delete this->smoothKeyFrame;
		this->smoothKeyFrame = NULL;
	}
}

SkinnedAnimation* SkinnedAnimation::clone(){
	SkinnedAnimation* clone = new SkinnedAnimation(this->name);

	for(unsigned int i=0; i < this->keyFrames.size(); i++){
		clone->addKeyFrame( this->keyFrames[i]->clone() );
	}
	return clone;
}

void SkinnedAnimation::nextKeyFrame(){
	if(this->smoothKeyFrame != NULL && !this->smoothKeyFrameExecuted && this->actualKeyIndex==0){
		this->keyFramesInterpolator.setInitialPose(this->smoothKeyFrame->getPoseSkeletonRoot());
		this->actualKeyFrameTime = (this->smoothKeyFrame->getDurationInSeconds() + this->keyFrames[this->actualKeyIndex]->getDurationInSeconds()) / 2.0f;
		this->smoothKeyFrameExecuted = true;
	}
	else{
		if(this->smoothKeyFrame != NULL){
			delete this->smoothKeyFrame;
			this->smoothKeyFrame = NULL;
		}
		this->smoothKeyFrameExecuted = false;
		this->keyFramesInterpolator.setInitialPose(this->keyFrames[this->actualKeyIndex]->getPoseSkeletonRoot());
		int keyIncrement = (-1) * this->reverseAnimationActive + (1 - this->reverseAnimationActive); // Incremento hacia adelante o atras dependiendo en que sentido animo
		this->actualKeyIndex += keyIncrement;  
		this->actualKeyFrameTime = (this->keyFrames[this->actualKeyIndex - keyIncrement]->getDurationInSeconds() + this->keyFrames[this->actualKeyIndex]->getDurationInSeconds()) / 2.0f;
	}
	this->keyFramesInterpolator.setEndPose(this->keyFrames[this->actualKeyIndex]->getPoseSkeletonRoot());	
	this->keyFramesInterpolator.getTimeInterpolator()->setInterpolationTime(this->actualKeyFrameTime * 1000);
	this->keyFramesInterpolator.getTimeInterpolator()->reset();
	this->keyFramesInterpolator.getTimeInterpolator()->setPlay();
}

void SkinnedAnimation::update(){
	bool hasToUpdate = true;
	// Si no tengo keyframe activo o ya termino el keyframe actual, activo el siguiente
	if((this->keyFramesInterpolator.getInitialPose() == NULL) || (this->keyFramesInterpolator.getTimeInterpolator()->getCurrentValue() >= 1.0f)){
		if(this->keyFrames.size() > this->actualKeyIndex + 1  && !this->reverseAnimationActive){		
			this->nextKeyFrame();
		}
		else if(this->actualKeyIndex > 0  && this->reverseAnimationActive){		
			this->nextKeyFrame();
		}
		else{
			if(this->hasLoop){
				this->reset();		
				this->nextKeyFrame();
			}
			else{				
				this->keyFramesInterpolator.setInitialPose(NULL);
				hasToUpdate = false;
			}			
		}
	}	
	// Si no termine la animación, continuo actualizando
	if(hasToUpdate){
		this->keyFramesInterpolator.update();
	}
}

void SkinnedAnimation::reset(){
	if(this->inverseAnimationLoopEnabled){
		this->reverseAnimationActive = !this->reverseAnimationActive;
	}
	else{
		this->actualKeyIndex = 0;
		this->reverseAnimationActive = false;
	}
}

MeshSkeleton* SkinnedAnimation::getActualPose(){
	return this->keyFramesInterpolator.getActualPose();
}

SkinnedAnimationKeyFrame* SkinnedAnimation::getActualKeyFrame(){
	SkinnedAnimationKeyFrame* keyframe = new SkinnedAnimationKeyFrame();
	if((this->keyFrames.size() > this->actualKeyIndex + 1  && !this->reverseAnimationActive) || (this->actualKeyIndex > 0  && this->reverseAnimationActive)){
		if(this->actualKeyIndex == NULL){
			this->update();
		}
		else{
			keyframe->setPoseSkeletonRoot(this->getActualPose()->clone());  // Uso el frame intgerpolado actual
		}
	}
	else{
		keyframe->setPoseSkeletonRoot(this->keyFrames.back()->getPoseSkeletonRoot()->clone());  // Ya termino la animacion uso el frame final
	}
	keyframe->setDurationInSeconds(this->actualKeyFrameTime);
	return keyframe;
}

void SkinnedAnimation::setTotalAnimationTime(float timeInSeconds){
	// Primero consigo el tiempo de animacion total entre todos los keyframes
	float totalTime = 0.0f;
	for(unsigned int i=0; i < this->keyFrames.size(); i++){
		totalTime += this->keyFrames[i]->getDurationInSeconds();
	}
	// Ahora pondero el tiempo de cada keyframe
	for(unsigned int i=0; i < this->keyFrames.size(); i++){
		float finalKeyframeTime = (this->keyFrames[i]->getDurationInSeconds() / totalTime) * timeInSeconds;
		this->keyFrames[i]->setDurationInSeconds(finalKeyframeTime);
	}
}

