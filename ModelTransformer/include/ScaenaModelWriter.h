/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ModelDataTransfer.h"
#include "XmlTree.h"
#include <string>
using namespace std;

class ScaenaModelWriter{

private:
	static XmlTree* modelToXmlTree(ModelDataTransfer* modelDataTransfer);
	static string getFinalFilename(string originalFilename);
	static void processMesh(XmlTreeNode* parentNode, MeshDataTransfer* meshDataTransfer);
	static void processSkeleton(XmlTreeNode* parentNode, SkeletonDataTransfer* skeletonDataTransfer);
	static void processJoint(XmlTreeNode* parentNode, SkeletonDataTransfer* skeletonDataTransfer);
	static void serializeTransformationsToNode(XmlTreeNode* parentNode, InformationDataTransfer* dataTransfer);
	static void serializeTexturesToNode(XmlTreeNode* parentNode, MeshDataTransfer* modelDataTransfer);
	static void ScaenaModelWriter::serializeSkiningInfoToNode(XmlTreeNode* parentNode, MeshDataTransfer* meshDataTransfer);
	
	static void meshToXmlTree(XmlTreeNode* parentNode, ModelDataTransfer* modelDataTransfer);
	static void skeletonToXmlTree(XmlTreeNode* parentNode, ModelDataTransfer* modelDataTransfer);

public:
	static void writeModel(string filename, ModelDataTransfer* modelDataTransfer);

};