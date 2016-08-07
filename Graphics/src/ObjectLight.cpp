/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ObjectLight.h"

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform2.hpp> 
#include <glm/gtx/projection.hpp>
typedef glm::mat4 mat4;

void ObjectLight::setLocalPosition(vec3 position){
	this->localPosition = position;
	// Recalculo la posicion absoluta y la direccion
	this->internalUpdateLightPositionAndDirection();
}

void ObjectLight::setDirection(vec3 value){ 
	this->unrotatedDirection = value; 
	// Recalculo la posicion absoluta y la direccion
	this->internalUpdateLightPositionAndDirection();
}

void ObjectLight::internalUpdateLightPositionAndDirection(){
	// Calculo la rotacion del objeto para aplicarsela tambien a la luz
	mat4 rotationMatrix = glm::rotate(mat4(1.0f),this->objectRotation.z, vec3(0.0f, 0.0f, 1.0f));
	rotationMatrix = glm::rotate(rotationMatrix, this->objectRotation.y, vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, this->objectRotation.x, vec3(1.0f, 0.0f, 0.0f));
	
	// Calculo la posicion absoluta
	vec3 finalLocalPosition = vec3(rotationMatrix * vec4(localPosition, 1.0f));
	this->absolutePosition = this->objectPosition + finalLocalPosition;

	// Calculo la direccion final
	this->direction = vec3(rotationMatrix * vec4(this->unrotatedDirection, 1.0f));
}

// Cuando el objecto cambia de posicion se debe mover tambien la luz
void ObjectLight::updateLightPositionAndDirection(vec3 objectPosition, vec3 objectRotation){
	this->objectPosition = objectPosition;
	this->objectRotation = objectRotation;
	this->internalUpdateLightPositionAndDirection();
}

// Clona la luz actual
SceneLight* ObjectLight::clone(){
	ObjectLight* clone = new ObjectLight();
	clone->setLocalPosition(this->localPosition);
	clone->setActive(this->active);
	clone->setDirection(this->direction);
	clone->setIntensity(this->intensity);
	clone->setLightType(this->lightType);
	clone->setSpotFactor(this->spotFactor);
	this->getShadowData()->clone(clone->getShadowData());	
	return clone;
}