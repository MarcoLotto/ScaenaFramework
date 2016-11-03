/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include <string.h>
#include "ScaenaAnimationLoader.h"
#include "XmlUtils.h"
#include "AnimationLoadException.h"
#include "WindowConnector.h"
#include "ScaenaModelLoaderHelper.h"
#include "DataConformationException.h"

void validateScaenaAnimationFile(const char* filename){
	int size = strlen(filename);
	if(filename[size-3]=='s' && filename[size-2]=='a' && filename[size-1]=='f')
		return;
	if(filename[size-3]=='S' && filename[size-2]=='A' && filename[size-1]=='F')
		return;
	throw new AnimationLoadException("El archivo " + string(filename) + " no es un .saf valido");
}

string getAdaptedScaenaAnimationFile(const char* filename){
	string finalFilename = string(filename);
	int size = strlen(filename);
	if(filename[size-3]=='s' && filename[size-2]=='m' && filename[size-1]=='f')
		finalFilename[size-2] = 'a';
	if(filename[size-3]=='S' && filename[size-2]=='M' && filename[size-1]=='F')
		finalFilename[size-2] = 'A';	
	return finalFilename;
}

void ScaenaAnimationLoader::loadAnimations(const char* filename, MeshSkeleton* skeleton){
	// Intento adaptar el nombre si se trata de un smf
	string safFilename = getAdaptedScaenaAnimationFile(filename);

	// Valido que el tipo de archivo sea correcto
	validateScaenaAnimationFile(safFilename.c_str());

	//Leo el archivo y lo parseo en un arbol
	XmlTree* xmlTree = XmlParser::parse((WindowConnector::getBaseApplicationPath() + safFilename).c_str());
	XmlTreeNode* root =  xmlTree->getRootNode();
	
	//Primiero busco la etiqueta de ScaenaAnimationFile
	XmlTreeNode* fileNode = NULL;
	for(int i=0; i < root->getChildNodes().size(); i++){
		fileNode = root->getChildNodes()[i];
		if(fileNode->getName().compare("ScaenaAnimationFile")==0){
			break;
		}
	}
	if(fileNode == NULL){
        if(xmlTree != NULL){ delete xmlTree; }
		return;
	}
	// Proceso cada uno de los hijos
	unsigned int iteration = 0;
	XmlTreeNode* animationNode = fileNode->searchDirectChild("Animation", 0);
	while(animationNode != NULL){
		processAnimation(animationNode, skeleton);
		iteration++;
		animationNode = fileNode->searchDirectChild("Animation", iteration);
	}
    delete xmlTree;
}

SkinnedAnimationKeyFrame* ScaenaAnimationLoader::loadKeyframeFromFile(const char* filename, MeshSkeleton* skeleton){
	//Leo el archivo y lo parseo en un arbol
	XmlTree* xmlTree = XmlParser::parse((WindowConnector::getBaseApplicationPath() + string(filename)).c_str());
	XmlTreeNode* root =  xmlTree->getRootNode();

	XmlTreeNode* fileNode = NULL;
	for(int i=0; i < root->getChildNodes().size(); i++){
		fileNode = root->getChildNodes()[i];
		if(fileNode->getName().compare("KeyFrame")==0){
			break;
		}
	}
	if(fileNode == NULL){
		return NULL;
	}
	return processKeyFrame(fileNode, skeleton);
}

void ScaenaAnimationLoader::processAnimation(XmlTreeNode* animationNode, MeshSkeleton* skeleton){	

	//Guardo el nombre de la animacion
	XmlNodeAttribute* attribute = animationNode->searchForAttribute("Name");
	if(attribute == NULL){
		throw new AnimationLoadException("Falta el atributo Name en la declaracion de una animacion");
	}
	SkinnedAnimation* animation = new SkinnedAnimation( attribute->getValue() );
	
	// Proceso todos los keyframes de la misma
	unsigned int iteration = 0;
	XmlTreeNode* keyFrameNode = animationNode->searchDirectChild("KeyFrame", 0);
	while(keyFrameNode != NULL){
		animation->addKeyFrame( processKeyFrame(keyFrameNode, skeleton) );
		iteration++;
		keyFrameNode = animationNode->searchDirectChild("KeyFrame", iteration);
	}

	// Me guardo la animación en el SkinnedAnimationRepository
	SkinnedAnimationRepository::getInstance()->addAnimation(animation);
}

SkinnedAnimationKeyFrame* ScaenaAnimationLoader::processKeyFrame(XmlTreeNode* keyFrameNode, MeshSkeleton* skeleton){	

	// Validamos que la animacion pueda ser aplicada al skeleton provisto
	XmlNodeAttribute* attribute = keyFrameNode->searchForAttribute("SkeletonHash");
	if(attribute == NULL){
		throw new AnimationLoadException("Falta el atributo SkeletonHash en la declaracion de un keyframe");
	}
	if(skeleton->getHash() != atoi(attribute->getValue().c_str())){
		throw new AnimationLoadException("La animacion no puede ser aplicada al skeleton dado. Hash de estructura difiere.");
	}
	//Guardo la duracion del keyframe
	attribute = keyFrameNode->searchForAttribute("DurationInSeconds");
	if(attribute == NULL){
		throw new AnimationLoadException("Falta el atributo DurationInSeconds en la declaracion de un keyframe");
	}
	SkinnedAnimationKeyFrame* keyframe = new SkinnedAnimationKeyFrame();
	keyframe->setDurationInSeconds(atof(attribute->getValue().c_str()));

	// Conseguimos y procesamos la data de la animación	
	keyframe->setPoseSkeletonRoot( processAnimationData(keyFrameNode, skeleton) );
	return keyframe;
}

MeshSkeleton* ScaenaAnimationLoader::processAnimationData(XmlTreeNode* keyFrameNode, MeshSkeleton* skeleton){	
	MeshSkeleton* animatedSkeleton = skeleton->clone();
	int bufferSize = 0;
	float* buffer = XmlUtils::createFloatBufferFromBinaryNode("Data", keyFrameNode, &bufferSize);	
	if(buffer == NULL){
		throw new AnimationLoadException("Error al procesar la data de un keyframe (Esta el tag de Data? El tamaño es correcto?)");
	}
	// Llenamos nuestro nuevo esqueleto con la informacion de la animación
	int actualIndex = -1;
	for(unsigned int i=0; i < animatedSkeleton->getChilds()->size(); i++){
		fillSkeletonJointWithAnimationData(animatedSkeleton->getChilds()->at(i), buffer, bufferSize, &actualIndex);
	}
	return animatedSkeleton;
}

void ScaenaAnimationLoader::fillSkeletonJointWithAnimationData(MeshSkeletonJoint* joint, float* buffer, int bufferSize, int* actualIndex){
	(*actualIndex)++;
	// Identificamos a que indices tenemos que acceder en el buffer y cargamos la data en el joint
	int initialBufferIndex = (*actualIndex) * 9;
	if(initialBufferIndex + 9 > bufferSize){
		throw new DataConformationException("Error al cargar animacion en skeleton. Se intento acceder a un indice mayor al buffer disponible");
	}
	joint->getGlobalBindposeTransformation()->setPosition(vec3(buffer[initialBufferIndex], buffer[initialBufferIndex + 1], buffer[initialBufferIndex + 2]));
	joint->getGlobalBindposeTransformation()->setRotation(vec3(buffer[initialBufferIndex + 3], buffer[initialBufferIndex + 4], buffer[initialBufferIndex + 5]));
	joint->getGlobalBindposeTransformation()->setScale(vec3(buffer[initialBufferIndex + 6], buffer[initialBufferIndex + 7], buffer[initialBufferIndex + 8]));

	// Procesamos todos los hijos del joint
	for(unsigned int i=0; i < joint->getChilds()->size(); i++){
		fillSkeletonJointWithAnimationData(joint->getChilds()->at(i), buffer, bufferSize, actualIndex);
	}
}


