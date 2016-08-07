/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "AnimationDataTransfer.h"
#include "XmlTree.h"
#include <string>
using namespace std;

class ScaenaAnimationWriter{

private:
	static XmlTree* animationsToXmlTree(list<AnimationDataTransfer*>* animations);
	static string getFinalFilename(string originalFilename);
	
	static void processSkeleton(XmlTreeNode* parentNode, SkeletonDataTransfer* skeletonDataTransfer);
	static void processJoint(XmlTreeNode* parentNode, SkeletonDataTransfer* skeletonDataTransfer);

	static void serializeTransformationsToNode(XmlTreeNode* parentNode, InformationDataTransfer* dataTransfer);	
	
	static void keyFrameToXmlTree(XmlTreeNode* parentNode, KeyFrameDataTransfer* keyframeDataTransfer);
	
public:
	static void writeAnimation(string filename, list<AnimationDataTransfer*>* animations);
	static void skeletonToXmlTree(XmlTreeNode* parentNode, SkeletonDataTransfer* skeletonDataTransfer);

};