/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ScaenaAnimationFileWriter.h"
#include "SkinnedAnimationKeyFrame.h"
#include "StringUtils.h"
#include "XmlUtils.h"
#include "XmlTree.h"
#include "XmlParser.h"
#include "WindowConnector.h"

void ScaenaAnimationFileWriter::writeKeyFrameToFileSystem(string filename, SkinnedAnimationKeyFrame* keyframe){
	XmlTree* xmlTree = new XmlTree();
	XmlTreeNode* node = xmlTree->getRootNode();
	this->keyFrameToXmlTree(node, keyframe);
	XmlParser::writeXmlToFile((WindowConnector::getBaseApplicationPath() + string(filename)).c_str(), xmlTree);
}

void ScaenaAnimationFileWriter::keyFrameToXmlTree(XmlTreeNode* parentNode, SkinnedAnimationKeyFrame* keyframe){
	// Agrego el encabezado de keyframe
	XmlTreeNode* keyframeNode = new XmlTreeNode("KeyFrame", parentNode);
	parentNode->addChild(keyframeNode);

	// Agrego como atributo la duracion del keyframe
	keyframeNode->addAttribute(new XmlNodeAttribute("DurationInSeconds", StringUtils::toString(keyframe->getDurationInSeconds())));

	// Agregamos el hash del skeleton al que pertenece
	keyframeNode->addAttribute(new XmlNodeAttribute("SkeletonHash", StringUtils::toString(keyframe->getPoseSkeletonRoot()->getHash())));

	// Agrego la pose
	this->processSkeleton(keyframeNode, keyframe->getPoseSkeletonRoot());
}

void ScaenaAnimationFileWriter::processSkeleton(XmlTreeNode* parentNode, MeshSkeleton* skeleton){
	vector<float> bufferMetadata;
	// Recorro todos los joints hijos y proceso cada uno
	vector<MeshSkeletonJoint*>::iterator it = skeleton->getChilds()->begin();
	while(it != skeleton->getChilds()->end()){
		processJoint(*it, &bufferMetadata);
		++it;
	}
	// Creamos el buffer final a grabar a partir de la metadata
	float* buffer = createBufferFromMetadata(&bufferMetadata);

	// Mandamos a serializar en binario y agregar al nodo
	XmlUtils::parseBufferInBinaryToNode("Data", buffer, bufferMetadata.size(), parentNode);  
	delete buffer;
}

void ScaenaAnimationFileWriter::processJoint(MeshSkeletonJoint* skeleton, vector<float>* bufferMetadata){

	// Guardo las transaformaciones de este joint en el vector de metadata para luego pasarlo a un buffer y serializar
	addTransformationsToMetadata(bufferMetadata, skeleton);

	// Recorro todos los hijos y proceso cada uno
	vector<MeshSkeletonJoint*>::iterator it = skeleton->getChilds()->begin();
	while(it != skeleton->getChilds()->end()){
		processJoint(*it, bufferMetadata);
		++it;
	}	
}

void ScaenaAnimationFileWriter::addTransformationsToMetadata(vector<float>* bufferMetadata, MeshSkeletonJoint* skeleton){
	vec3 position = skeleton->getGlobalBindposeTransformation()->getPosition();
	vec3 rotation = skeleton->getGlobalBindposeTransformation()->getRotation();
	vec3 scale = skeleton->getGlobalBindposeTransformation()->getScale();

	bufferMetadata->push_back(position.x);
	bufferMetadata->push_back(position.y);
	bufferMetadata->push_back(position.z);

	bufferMetadata->push_back(rotation.x);
	bufferMetadata->push_back(rotation.y);
	bufferMetadata->push_back(rotation.z);

	bufferMetadata->push_back(scale.x);
	bufferMetadata->push_back(scale.y);
	bufferMetadata->push_back(scale.z);
}

float* ScaenaAnimationFileWriter::createBufferFromMetadata(vector<float>* bufferMetadata){
	float* buffer = new float[bufferMetadata->size()];
	for(unsigned int i=0; i < bufferMetadata->size(); i++){
		buffer[i] = bufferMetadata->at(i);
	}
	return buffer;
}