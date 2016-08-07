/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ScaenaAnimationWriter.h"
#include "SkeletonDataTransfer.h"
#include "XmlParser.h"
#include "XmlUtils.h"
#include "StringUtils.h"
#include "Logger.h"

void ScaenaAnimationWriter::writeAnimation(string filename, list<AnimationDataTransfer*>* animations){
	Logger::getInstance()->logInfo(new Log("Procesando el SAF de salida..."));
	XmlTree* xmlTree = animationsToXmlTree(animations);
	XmlParser::writeXmlToFile(getFinalFilename(filename).c_str(), xmlTree);
}

string ScaenaAnimationWriter::getFinalFilename(string originalFilename){
	return originalFilename.erase(originalFilename.length()-3, originalFilename.length()) + "saf";
}

XmlTree* ScaenaAnimationWriter::animationsToXmlTree(list<AnimationDataTransfer*>* animations){
	XmlTree* xmlTree = new XmlTree();
	XmlTreeNode* rootNode = xmlTree->getRootNode();

	// Agrego el tag inicial
	XmlTreeNode* safNode = new XmlTreeNode("ScaenaAnimationFile", rootNode);
	rootNode->addChild(safNode);

	// Agrego todas las animaciones
	list<AnimationDataTransfer*>::iterator animIt = animations->begin();
	while( animIt != animations->end() ){

		// Agrego el tag de animacion
		XmlTreeNode* animationNode = new XmlTreeNode("Animation", safNode);
		safNode->addChild(animationNode);

		// Agrego como atributo el nombre de la animacion
		AnimationDataTransfer* animationDataTransfer = *animIt;
		animationNode->addAttribute(new XmlNodeAttribute("Name", animationDataTransfer->name));

		// Agrego cada uno de los keyframes
		list<KeyFrameDataTransfer*>::iterator keyIt = animationDataTransfer->keyframes->begin();
		while(keyIt != animationDataTransfer->keyframes->end()){
			keyFrameToXmlTree(animationNode, *keyIt);
			++keyIt;
		}	
		++animIt;
	}
	return xmlTree;
}

void ScaenaAnimationWriter::keyFrameToXmlTree(XmlTreeNode* parentNode, KeyFrameDataTransfer* keyFrameDataTransfer){
	// Agrego el encabezado de keyframe
	XmlTreeNode* keyframeNode = new XmlTreeNode("KeyFrame", parentNode);
	parentNode->addChild(keyframeNode);

	// Agrego como atributo la duracion del keyframe
	keyframeNode->addAttribute(new XmlNodeAttribute("DurationInSeconds", StringUtils::toString(keyFrameDataTransfer->animationTime)));

	// Agrego la pose
	skeletonToXmlTree(keyframeNode, keyFrameDataTransfer->getRootSkeleton());
}

void ScaenaAnimationWriter::skeletonToXmlTree(XmlTreeNode* parentNode, SkeletonDataTransfer* skeletonDataTransfer){
	// Consigo el nodo root del esqueleto
	SkeletonDataTransfer* rootSkeletonNode = skeletonDataTransfer;

	if(rootSkeletonNode != NULL){
		// Recorro todos los hijos y proceso cada uno
		list<SkeletonDataTransfer*>::iterator it = rootSkeletonNode->getChilds()->begin();
		while(it != rootSkeletonNode->getChilds()->end()){
			processSkeleton(parentNode, *it);
			++it;
		}
	}
}

void ScaenaAnimationWriter::processSkeleton(XmlTreeNode* parentNode, SkeletonDataTransfer* skeletonDataTransfer){
	// Creo un nuevo nodo para el mesh
	XmlTreeNode* skeletonNode = new XmlTreeNode("Skeleton", parentNode);
	parentNode->addChild(skeletonNode);
	skeletonNode->addAttribute(new XmlNodeAttribute("Name", skeletonDataTransfer->jointName));	
	
	// Recorro todos los hijos y proceso cada uno
	list<SkeletonDataTransfer*>::iterator it = skeletonDataTransfer->getChilds()->begin();
	while(it != skeletonDataTransfer->getChilds()->end()){
		processJoint(skeletonNode, *it);
		++it;
	}	
}

void ScaenaAnimationWriter::processJoint(XmlTreeNode* parentNode, SkeletonDataTransfer* skeletonDataTransfer){
	// Creo un nuevo nodo para el mesh
	XmlTreeNode* jointNode = new XmlTreeNode("Joint", parentNode);
	parentNode->addChild(jointNode);
	jointNode->addAttribute(new XmlNodeAttribute("Name", skeletonDataTransfer->jointName));
	jointNode->addAttribute(new XmlNodeAttribute("Index", StringUtils::toString(skeletonDataTransfer->jointIndex - 2)));  // -2 es porque no se cuenta ni el dt root ni el skeleton root
	
	// Me guardo las transformaciones (globales) del joint a la hora de cuando fue bindeado con el mesh
	serializeTransformationsToNode(jointNode, skeletonDataTransfer);

	// Recorro todos los hijos y proceso cada uno
	list<SkeletonDataTransfer*>::iterator it = skeletonDataTransfer->getChilds()->begin();
	while(it != skeletonDataTransfer->getChilds()->end()){
		processJoint(jointNode, *it);
		++it;
	}	
}

void ScaenaAnimationWriter::serializeTransformationsToNode(XmlTreeNode* parentNode, InformationDataTransfer* dataTransfer){
	XmlTreeNode* transformationNode = new XmlTreeNode("Transformations", parentNode);
	parentNode->addChild(transformationNode);
	// Las propiedades hijas		
	XmlUtils::serializeToNode("Translation", dataTransfer->position, transformationNode);
	XmlUtils::serializeToNode("Rotation", dataTransfer->rotation, transformationNode);
	XmlUtils::serializeToNode("Scale", dataTransfer->scale, transformationNode);
}




