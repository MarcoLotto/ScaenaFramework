/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "CamaraTerceraPersona.h"
#include "TimeManager.h"

#define PI 3.141516f

CamaraTerceraPersona::CamaraTerceraPersona(){
	this->distanceToPoint = 0.0f;
	this->zoomTimeSlot = TimeManager::getInstance()->reservTimeSlot();
	this->zoomAmout = 0.0f;
	this->zoomFocusTime = 0;
	this->zoomTotalTime = 0;
}
CamaraTerceraPersona::~CamaraTerceraPersona(){
	TimeManager::getInstance()->removeTimeSlot(this->zoomTimeSlot);
}
void CamaraTerceraPersona::update(){
	//Translado respecto al origen la separacion al punto (Sist de coord: z=Up, x=side, y=front)
	mat4 view = glm::translate(glm::mat4(1.0f), vec3(0.0f, 0.0f, 1.0f) * this->getUpdateDistanceToPoint());
	
	//Roto al rededor del origen
	//Yaw
	view = glm::rotate(view, this->eyeRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	//Pitch
	view = glm::rotate(view, this->eyeRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//Roll
	view = glm::rotate(view, this->eyeRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	// Genero la rotacion externa a la camara
	mat4 externalView = glm::rotate(mat4(1.0f), this->externalEyePointRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	externalView = glm::rotate(externalView, this->externalEyePointRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	externalView = glm::rotate(externalView, this->externalEyePointRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

	// Aplico la matriz de rotacion externa
	view = view * glm::inverse(externalView);
	
	//Translado el ojo ya rotado hasta la posicion del punto lookAt
	view = glm::translate(view, -this->lookAt);

	//Guardo el resultado en la view matrix
	this->viewMatrix = view;
	//Consigo la posicion del ojo (camara)
	this->eyePosition = vec3(glm::inverse(this->viewMatrix) * vec4( 0.0f, 0.0f, 0.0f, 1.0f));

	// Actualizo el viewing frustum
	vec3 lookAtDirection = glm::normalize(this->lookAt - this->eyePosition);
	this->updateDirectionVectors(lookAtDirection);
	this->viewFrustum->update(this->eyePosition, lookAtDirection, this->upDireccion, this->sideDireccion);
}

void CamaraTerceraPersona::updateDirectionVectors(vec3 lookAtDirection){		
	vec3 up = vec3(0.0f, 0.0f, 1.0f);
	vec3 side = vec3(1.0f, 0.0f, 0.0f);	
	float angleSide = rotationAngleBetweenVectors(lookAtDirection, side);
	mat4 rotMat = glm::rotate(mat4(1.0f), angleSide, vec3(1.0f, 0.0f, 0.0f));		
	this->upDireccion = vec3(rotMat * vec4(up, 1.0f));	
	this->sideDireccion = glm::cross(this->upDireccion, lookAtDirection);
}

float CamaraTerceraPersona::norm(vec3 v){
	return glm::sqrt((v.x)*(v.x)+ (v.y)*(v.y) + (v.z)*(v.z));
}

float CamaraTerceraPersona::rotationAngleBetweenVectors(vec3 u, vec3 v){
	float angle = glm::dot(u, v) / (norm(u) * norm(v));
	angle = glm::acos(angle);
	angle -= PI / 2.0f;  //los quiero perpendiculares
	return (angle/PI)*180.0f;
}

void CamaraTerceraPersona::setDistanceToPoint(float distance){
	this->distanceToPoint = distance;
}
float CamaraTerceraPersona::getDistanceToPoint(){
	return this->distanceToPoint;
}
void CamaraTerceraPersona::setLookAtPoint(vec3 lookAt){
	this->lookAt = lookAt;
}

void CamaraTerceraPersona::rotateEyePoint(vec3 rotation){
	this->externalEyePointRotation = rotation;
}

void CamaraTerceraPersona::makeZoom(float zoomAmount, int focusTimeMilliseconds, int zoomTimeMilliseconds){
	this->zoomFocusTime = focusTimeMilliseconds;
	this->zoomTotalTime = zoomTimeMilliseconds;
	this->zoomAmout = zoomAmount;
	TimeManager::getInstance()->updateTimeSlot(this->zoomTimeSlot);
}

float CamaraTerceraPersona::getUpdateDistanceToPoint(){
	int elapsedTime = TimeManager::getInstance()->getElapsedTime(this->zoomTimeSlot);
	// No tenemos zoom, terminamos aca
	if(elapsedTime > this->zoomTotalTime){
		return this->distanceToPoint;
	}
	// Estamos por terminar el zoom, sacamos el foco	
	if(elapsedTime > this->zoomTotalTime - this->zoomFocusTime){
		float interval = (this->zoomTotalTime - elapsedTime) / (this->zoomTotalTime - this->zoomFocusTime);
		return this->distanceToPoint + interval * this->zoomAmout;
	}
	// Ya estamos en zoom correcto, mantenemos ese zoom
	if(elapsedTime > this->zoomFocusTime){
		return this->distanceToPoint + this->zoomAmout;
	}
	// Estoy haciendo foco hacia el zoom, calculo el foco correspondiente
	float interval = 1.0f - ((this->zoomFocusTime - elapsedTime) / this->zoomFocusTime);
	return this->distanceToPoint + interval * this->zoomAmout;
}

float CamaraTerceraPersona::getEyeRotationPitch(){ 
	return this->externalEyePointRotation.x; 
}	
float CamaraTerceraPersona::getEyeRotationYaw(){ 
	return this->externalEyePointRotation.z; 
}
float CamaraTerceraPersona::getEyeRotationRoll(){ 
	return this->externalEyePointRotation.y; 
}