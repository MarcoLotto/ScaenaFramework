/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshSkeleton.h"

class SkinnedAnimationKeyFrame{

private:
	float durationInSeconds;
	MeshSkeleton* poseSkeletonRoot;

public:
	SkinnedAnimationKeyFrame(){ this->durationInSeconds = 0.0f; this->poseSkeletonRoot = NULL; }
	virtual ~SkinnedAnimationKeyFrame();

	// La duracion del keyframe en segundos
	void setDurationInSeconds(float duration){ this->durationInSeconds = duration; }
	float getDurationInSeconds(){ return this->durationInSeconds; }

	// El esqueleto de la pose en este key frame
	void setPoseSkeletonRoot(MeshSkeleton* skeleton){ this->poseSkeletonRoot = skeleton; }
	MeshSkeleton* getPoseSkeletonRoot(){ return this->poseSkeletonRoot; }

	SkinnedAnimationKeyFrame* clone();
};