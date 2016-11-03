/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "DirectionalMovingObject.h"

// El objeto en cuestión y el ajuste respecto de la rotacion original del objeto para que coincidan los ejes de yaw-pitch-roll)
DirectionalMovingObject::DirectionalMovingObject(Object* object, vec3 rotationToAjustAxis){
	// Me guardo el objeto real
	this->object = object;

	//Modifico la rotación del objeto con las coordenadas pasadas
	vec3 initialRotation = this->object->getTransformation()->getRotation();
	this->object->getTransformation()->setRotation(initialRotation + rotationToAjustAxis);

	// Inicializo los vectores
	this->sideDirection = vec3(1.0f, 0.0f, 0.0f);
	this->lookAt = vec3(0.0f, 1.0f, 0.0f);
	this->upDirection = vec3(0.0f, 0.0f, 1.0f);	
}

// Rotaciones en cada eje del objeto
void DirectionalMovingObject::rotate(vec3 rotationDegrees){
	// Primero genero la rotacion de los tres vectores de yaw-Pitch-Roll
	mat4 rotationMatrix = glm::rotate(mat4(1.0f), rotationDegrees.z, glm::vec3(0.0f, 0.0f, 1.0f));
	rotationMatrix = glm::rotate(rotationMatrix, rotationDegrees.y, glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, rotationDegrees.x, glm::vec3(1.0f, 0.0f, 0.0f));

	//Consigo la nueva direccion de los vectores lookAt, up y side
	this->lookAt = vec3(rotationMatrix * vec4( 0.0f, 1.0f, 0.0f, 1.0f));
	this->upDirection = vec3(rotationMatrix * vec4( 0.0f, 0.0f, 1.0f, 1.0f));
	this->sideDirection = glm::cross(this->lookAt, this->upDirection);

	// Por ultimo le digo al objeto que rote
	this->object->getTransformation()->setRotation(rotationDegrees);
}
	
// Translación en cada eje del objeto
void DirectionalMovingObject::goForward(float meters){
	vec3 originalPosition = object->getTransformation()->getPosition();
	this->object->getTransformation()->setPosition(originalPosition + meters * this->lookAt);
}
void DirectionalMovingObject::goBackward(float meters){
	this->goForward(-meters);
}
void DirectionalMovingObject::goLeft(float meters){
	vec3 originalPosition = object->getTransformation()->getPosition();
	this->object->getTransformation()->setPosition(originalPosition + meters * this->sideDirection);
}
void DirectionalMovingObject::goRight(float meters){
	this->goLeft(-meters);
}
void DirectionalMovingObject::goUp(float meters){
	vec3 originalPosition = object->getTransformation()->getPosition();
	this->object->getTransformation()->setPosition(originalPosition + meters * this->upDirection);
}
void DirectionalMovingObject::goDown(float meters){
	this->goUp(-meters);
}

	