/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "CamaraPrimeraPersona.h"

CamaraPrimeraPersona::CamaraPrimeraPersona(){
	//Establezco un lookAt como direccion	
	this->sideDireccion = vec3(1.0f, 0.0f, 0.0f); 
}
void CamaraPrimeraPersona::update(){
	//Roto al rededor del origen
	//Yaw
	mat4 view = glm::rotate(mat4(1.0f), this->eyeRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	//Pitch
	view = glm::rotate(view, this->eyeRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//Roll
	view = glm::rotate(view, this->eyeRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	//Consigo la nueva direccion de los vectores lookAt, up y side
	this->lookAt = vec3(glm::inverse(view) * vec4( 0.0f, 0.0f, 1.0f, 1.0f));
	this->upDireccion = vec3(glm::inverse(view) * vec4( 0.0f, 1.0f, 0.0f, 1.0f));
	this->sideDireccion = glm::cross(this->lookAt, this->upDireccion);

	//Translado el ojo, ya rotado, a la posicion correcta
	view = glm::translate(view, -this->eyePosition);
	//Guardo el resultado en la view matrix
	this->viewMatrix = view;

	// Actualizo el viewing frustum
	this->viewFrustum->update(this->eyePosition, -this->lookAt, this->upDireccion, this->sideDireccion);
}
void CamaraPrimeraPersona::moveFront(float velocity){
	this->eyePosition -= this->lookAt * velocity;	
}
void CamaraPrimeraPersona::moveSide(float velocity){
	this->eyePosition -= this->sideDireccion * velocity;
}
void CamaraPrimeraPersona::moveUp(float velocity){
	this->eyePosition -= this->upDireccion * velocity;
}
void CamaraPrimeraPersona::setEyePosition(vec3 position){
	this->eyePosition = position;
}