/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SkinnedAnimationRepository.h"

SkinnedAnimationRepository* SkinnedAnimationRepository::instance = NULL;

SkinnedAnimationRepository* SkinnedAnimationRepository::getInstance(){
	if(instance == NULL){
		instance = new SkinnedAnimationRepository();
	}
	return instance;
}

// Agrega una animación al repositorio
void SkinnedAnimationRepository::addAnimation(SkinnedAnimation* animation){
	if(animation->getKeyFrames()->size() == 0)
		return;

	// Me consigo el esqueleto de uno de sus keyframes (puede ser cualquiera)
	unsigned int animationSkeletonHash = animation->getKeyFrames()->at(0)->getPoseSkeletonRoot()->getHash();

	// Busco si ya existe la entrada con el skeleton para esta animacion, de no ser asi la creo
	map<unsigned int, map<string, SkinnedAnimation*>*>::iterator animationMethodsIt = this->animationMethods.find(animationSkeletonHash);
	if(animationMethodsIt == this->animationMethods.end()){
		this->animationMethods[animationSkeletonHash] = new map<string, SkinnedAnimation*>();
	}
	// Ahora verifico si ya existe el metodo de animacion para este skeleton. De ser asi no agrego nada.
	map<string, SkinnedAnimation*>* animationsMap = this->animationMethods[animationSkeletonHash];
	map<string, SkinnedAnimation*>::iterator animationIt = animationsMap->find(animation->getName());		
	if(animationIt == animationsMap->end()){
		animationsMap->insert( std::pair<string, SkinnedAnimation*>(animation->getName(), animation) );
	}
}

SkinnedAnimation* SkinnedAnimationRepository::getAnimation(MeshSkeleton* skeleton, string animationName){
	// Primero busco los metodos de animacion disponibles para el esquelto buscado
	map<unsigned int, map<string, SkinnedAnimation*>*>::iterator animationMethodsIt = this->animationMethods.find(skeleton->getHash());
	if(animationMethodsIt != this->animationMethods.end()){
		// Ahora busco el metodo de animacion que me pidieron
		map<string, SkinnedAnimation*>* animationsMap = animationMethodsIt->second;
		map<string, SkinnedAnimation*>::iterator animationIt = animationsMap->find(animationName);		
		if(animationIt != animationsMap->end()){
			return (animationIt->second)->clone();
		}
	}
	return NULL;
}

// Borra (limpia fisicamente) todo el contenido del repositorio
void SkinnedAnimationRepository::cleanRepository(){
	map<unsigned int, map<string, SkinnedAnimation*>*>::iterator animationMethodsIt = this->animationMethods.begin();
	while(animationMethodsIt != this->animationMethods.end()){
		map<string, SkinnedAnimation*>* animationsMap = animationMethodsIt->second;
		map<string, SkinnedAnimation*>::iterator animationIt = animationsMap->begin();	
		while(animationIt != animationsMap->end()){
			delete animationIt->second;
			++animationIt;
		}
		delete animationsMap;
		animationMethodsIt = this->animationMethods.erase(animationMethodsIt);
	}
}