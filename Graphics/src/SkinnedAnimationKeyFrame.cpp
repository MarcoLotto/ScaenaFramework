/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SkinnedAnimationKeyFrame.h"

SkinnedAnimationKeyFrame::~SkinnedAnimationKeyFrame(){
	if(this->poseSkeletonRoot != NULL)
		delete this->poseSkeletonRoot;
}

SkinnedAnimationKeyFrame* SkinnedAnimationKeyFrame::clone(){
	SkinnedAnimationKeyFrame* clone = new SkinnedAnimationKeyFrame();
	clone->setDurationInSeconds(this->durationInSeconds);
	clone->setPoseSkeletonRoot(this->poseSkeletonRoot->clone());
	return clone;
}
