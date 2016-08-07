/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "GenerationProcessor.h"
#include "Logger.h"
#include "FbxModelLoader.h"
#include "ScaenaModelWriter.h"
#include "XmlParser.h"
#include "AnimationLoadException.h"
#include "ScaenaAnimationWriter.h"
#include "AnimationDataTransfer.h"
#include "SkeletonDataTransfer.h"
#include "KeyFrameBruteDataTransfer.h"

bool GenerationProcessor::processScaenaModelFile(const char* filename){
	//Parseo el archivo de modelo
	FbxModelLoader* fbxLoader = new FbxModelLoader();
	ModelDataTransfer* modelDataTransfer = fbxLoader->loadModel(filename);
	delete fbxLoader;

	if(modelDataTransfer == NULL){
		// Huston, tenemos un problema
		Logger::getInstance()->logError(new Log("Error al parsear el archivo de modelo de entrada"));
		return false;
	}

	// Escribo el Scaena Model File
	ScaenaModelWriter::writeModel(modelDataTransfer->filepath, modelDataTransfer);
	delete modelDataTransfer;
	
	// Hemos terminado!
	Logger::getInstance()->logInfo(new Log("Se ha convertido el modelo exitosamente!"));
	return true;
}

void skeletonToKeyframe(SkeletonDataTransfer* destinationJointDataTransfer, SkeletonDataTransfer* originJointDataTransfer, int keyFrameNumber){
	destinationJointDataTransfer->jointName = originJointDataTransfer->jointName;
	destinationJointDataTransfer->jointIndex = originJointDataTransfer->jointIndex;

	if(originJointDataTransfer->getKeyFrames()->size() > keyFrameNumber){
		// Consigo un keyframe del joint
		KeyFrameBruteDataTransfer* keyframeBruteData = originJointDataTransfer->getKeyFrames()->at(keyFrameNumber);
		destinationJointDataTransfer->position = keyframeBruteData->position;
		destinationJointDataTransfer->rotation = keyframeBruteData->rotation;
		destinationJointDataTransfer->scale = keyframeBruteData->scale;
	}

		
	// Recorro todos los hijos y proceso cada uno
	list<SkeletonDataTransfer*>::iterator it = originJointDataTransfer->getChilds()->begin();
	while(it != originJointDataTransfer->getChilds()->end()){
		SkeletonDataTransfer* joint = new SkeletonDataTransfer();
		skeletonToKeyframe(joint, *it, keyFrameNumber);
		destinationJointDataTransfer->addChild(joint);
		++it;
	}
}

KeyFrameDataTransfer* getKeyFrameData(ModelDataTransfer* modelDataTransfer, int keyFrameNumber){
	SkeletonDataTransfer* originSkeleton = modelDataTransfer->getRootSkeleton();	
	SkeletonDataTransfer* destinationSkeleton = new SkeletonDataTransfer();
	skeletonToKeyframe(destinationSkeleton, originSkeleton, keyFrameNumber);

	KeyFrameDataTransfer* keyframe = new KeyFrameDataTransfer();
	keyframe->setRootSkeleton(destinationSkeleton);
	//keyframe->animationTime = keyDuration;
	return keyframe;
}

bool GenerationProcessor::processScaenaAnimationFile(string& inAnimationFile, string& outAnimationFile, string& keyFrameFBXFile, string& animationName, float keyDuration){
	//Parseo el archivo de modelo
	FbxModelLoader* fbxLoader = new FbxModelLoader();
	ModelDataTransfer* modelDataTransfer = fbxLoader->loadModel(keyFrameFBXFile.c_str());
	KeyFrameDataTransfer* keyframe = getKeyFrameData(modelDataTransfer, 0);
	keyframe->animationTime = keyDuration;
	delete fbxLoader;

	// Si tengo SAF de entrada, lo cargo para agregarle la nueva animacion o keyframe
	if(!inAnimationFile.empty()){
		Logger::getInstance()->logInfo(new Log("Procesando el SAF de salida..."));

		//Leo el archivo y lo parseo en un arbol
		XmlTree* xmlTree = XmlParser::parse(inAnimationFile.c_str());
		XmlTreeNode* rootNode =  xmlTree->getRootNode();

		// Busco el nodo de archivo SAF
		XmlTreeNode* safNode = rootNode->searchDirectChild("ScaenaAnimationFile", 0);
		if(safNode != NULL){
			// Busco si alguna animacion tiene el mismo nombre que la indicada, si no creo una nueva
			XmlTreeNode* animationNode = searchForAnimation(safNode, animationName);
			if(animationNode == NULL){
				animationNode = new XmlTreeNode("Animation", safNode);
				animationNode->addAttribute(new XmlNodeAttribute("Name", animationName));
				safNode->addChild(animationNode);
			}
			// Agrego el keyframe
			XmlTreeNode* keyFrameNode = new XmlTreeNode("KeyFrame", animationNode);
			keyFrameNode->addAttribute(new XmlNodeAttribute("DurationInSeconds", StringUtils::toString(keyframe->animationTime)));
			ScaenaAnimationWriter::skeletonToXmlTree(keyFrameNode, keyframe->getRootSkeleton());			
			animationNode->addChild(keyFrameNode);			

			// Grabo en el archivo SAF (REVIEW: podria dejar de escribir cada cambio en disco)
			XmlParser::writeXmlToFile(outAnimationFile.c_str(), xmlTree);
		}
		else{
			throw new AnimationLoadException("No se puede procesar el SAF de entrada");
		}
	}
	// Si no tengo SAF de entrada, creo la estructura de cero
	else{
		list<AnimationDataTransfer*> animations;
		AnimationDataTransfer* animation = new AnimationDataTransfer();
		animation->name = animationName;
		animation->keyframes->push_back(keyframe);
		animations.push_back(animation);
		// Escribo el SAF e indico que ahora tengo archivo de entrada
		ScaenaAnimationWriter::writeAnimation(outAnimationFile, &animations);
		inAnimationFile = outAnimationFile;
	}	
	delete modelDataTransfer;
	delete keyframe;
	return true;
}

XmlTreeNode* GenerationProcessor::searchForAnimation(XmlTreeNode* safNode, string animationName){
	unsigned int iteration = 0;
	XmlTreeNode* animationNode = safNode->searchDirectChild("Animation", 0);
	while(animationNode != NULL){
		XmlNodeAttribute* nameAttribute = animationNode->searchForAttribute("Name");
		if(nameAttribute == NULL){
			throw new AnimationLoadException("Falta el atributo Name en una declaracion de animacion");
		}
		if(nameAttribute->getValue().compare(animationName)==0){
			return animationNode;
		}
		iteration++;
		animationNode = safNode->searchDirectChild("Animation", iteration);
	}
	return NULL;
}

