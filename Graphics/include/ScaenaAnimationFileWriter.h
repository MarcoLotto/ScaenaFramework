/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "XmlTreeNode.h"
#include "SkinnedAnimationKeyFrame.h"
#include "MeshSkeletonJoint.h"

class ScaenaAnimationFileWriter{

private:	
	int jointIndex;

	void processSkeleton(XmlTreeNode* parentNode, MeshSkeleton* skeleton);
	void processJoint(MeshSkeletonJoint* skeleton, vector<float>* bufferMetadata);
	void addTransformationsToMetadata(vector<float>* bufferMetadata, MeshSkeletonJoint* skeleton);
	float* createBufferFromMetadata(vector<float>* bufferMetadata);

public:
	void writeKeyFrameToFileSystem(string filename, SkinnedAnimationKeyFrame* keyframe);
	void keyFrameToXmlTree(XmlTreeNode* parentNode, SkinnedAnimationKeyFrame* keyframe);	

};