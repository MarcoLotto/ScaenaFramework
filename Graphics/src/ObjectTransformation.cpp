/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ObjectTransformation.h"

void ObjectTransformation::updateObjectLightsPosition(){		
	// Actualizo la posicion de las luces
	for(unsigned int i=0; i < this->objectAttachedLights->size(); i++){
		this->objectAttachedLights->at(i)->updateLightPositionAndDirection(this->getPosition(), this->getRotation());
	}
}
void ObjectTransformation::updateObjectLightsRotation(){
	// Actualizo la direccion de las luces
	for(unsigned int i=0; i < this->objectAttachedLights->size(); i++){
		this->objectAttachedLights->at(i)->updateLightPositionAndDirection(this->getPosition(), this->getRotation());
	}
}

ObjectTransformation::ObjectTransformation(vector<ObjectLight*>* objectAttachedLights){
	this->objectAttachedLights = objectAttachedLights;
}

void ObjectTransformation::setPosition(vec3 position){
	this->position = position;
	this->localRecalculationNeeded = true;
	this->updateObjectLightsPosition();
}

void ObjectTransformation::setRotation(vec3 rotation){
	this->rotation = rotation;
	this->localRecalculationNeeded = true;
	this->updateObjectLightsRotation();
}