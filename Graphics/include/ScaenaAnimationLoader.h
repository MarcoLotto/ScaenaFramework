/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "XmlParser.h"
#include "SkinnedAnimationKeyFrame.h"
#include "SkinnedAnimationRepository.h"
#include "MeshSkeletonJoint.h"

class Mesh;

class ScaenaAnimationLoader{

private:
	static void processAnimation(XmlTreeNode* animationNode, MeshSkeleton* skeleton);
	static SkinnedAnimationKeyFrame* processKeyFrame(XmlTreeNode* keyFrameNode, MeshSkeleton* skeleton);
	static MeshSkeleton* processAnimationData(XmlTreeNode* keyFrameNode, MeshSkeleton* skeleton);
	static void fillSkeletonJointWithAnimationData(MeshSkeletonJoint* joint, float* buffer, int bufferSize, int* actualIndex);
	
public:	
	// Carga un archivo saf de disco. Cada animacion se guarda en el SkinnedAnimationRepository.
	static void loadAnimations(const char* filename, MeshSkeleton* skeleton);
	static SkinnedAnimationKeyFrame* loadKeyframeFromFile(const char* filename, MeshSkeleton* skeleton);

};