/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "XmlParser.h"
#include "MeshDrawerShader.h"
#include "MeshSkeleton.h"
#include "Transformation.h"

class Mesh;

class ScaenaModelLoaderHelper{

public:		
	static MeshSkeleton* processSkeleton(XmlTreeNode* skeletonNode);
	static void processSkeletonJoint(XmlTreeNode* skeletonNode, MeshSkeleton* parentSkeleton);
	static void processMeshTransformations(string meshName, string meshFilePath, Transformation* transformation, XmlTreeNode* meshNode);

};