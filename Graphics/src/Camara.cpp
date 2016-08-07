/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "Camara.h"
#include "GenericUtils.h"

Camara::Camara(){
	this->viewFrustum = new ViewFrustum();
	this->configure();
}

Camara::~Camara(){
	delete this->viewFrustum;
}

//**Helpers***************************
float normalizarAngulo(float angle){
	while(angle > 360.0f)
		angle -= 360.0f;
	while(angle < -360.0f)
		angle += 360.0f;
	return angle;
}
//************************************

void Camara::configure(){
	this->eyePosition = vec3(0.0f, 0.0f, 0.0f);	
	this->eyeRotation = vec3(0.0f, 0.0f, 0.0f);
	this->setMaxRotationAngles(vec3(0.0f, 0.0f, 0.0f), vec3(360.0f, 360.0f, 360.0f));
	this->lookAt = vec3(0.0f, 0.0f, 1.0f);
	this->upDireccion = vec3(0.0f, 1.0f, 0.0f);
	this->sideDireccion = glm::cross(this->lookAt, this->upDireccion);
	this->viewMatrix = mat4(1.0f);
	this->setPerspective(70.0f, 1.5f, 0.1f, 1000.0f);
}

void Camara::setPerspective(float angle, float ratio, float nearD, float farD){
	this->perspectiveMatrix = glm::perspective(angle, ratio, nearD, farD);
	this->viewFrustum->setPerspectiveParameters(angle, ratio, nearD, farD);
}

vec3 Camara::getEyePosition(){
	return this->eyePosition;
}
vec3 Camara::getLookAtPoint(){
	return lookAt;
}
void Camara::setRotationPitch(float angle){	
	angle = normalizarAngulo(angle);
	if(angle <= this->baseRotationAngles.x + this->maxRotAngles.x){
		if(angle >= this->baseRotationAngles.x - this->maxRotAngles.x)
			this->eyeRotation.x = angle;
	}
}
void Camara::setRotationRoll(float angle){
	angle = normalizarAngulo(angle);
	if(angle <= this->baseRotationAngles.y + this->maxRotAngles.y){
		if(angle >= this->baseRotationAngles.y - this->maxRotAngles.y)
			this->eyeRotation.y = angle;
	}
}
void Camara::setRotationYaw(float angle){
	angle = normalizarAngulo(angle);
	if(angle <= this->baseRotationAngles.z + this->maxRotAngles.z){
		if(angle >= this->baseRotationAngles.z - this->maxRotAngles.z)
			this->eyeRotation.z = angle;
	}
}
float Camara::getEyeRotationPitch(){
	return this->eyeRotation.x;
}
float Camara::getEyeRotationYaw(){
	return this->eyeRotation.z;
}
float Camara::getEyeRotationRoll(){
	return this->eyeRotation.y;
}
void Camara::setMaxRotationAngles(vec3 PitchRollYawBase, vec3 PitchRollYawDelta){
	this->baseRotationAngles = PitchRollYawBase;
	this->maxRotAngles = PitchRollYawDelta;
}
void Camara::setUpDireccion(vec3 direccion){
	this->upDireccion = direccion;
}
vec3 Camara::getUpDireccion(){
	return this->upDireccion;
}
mat4 Camara::getViewMatrix(){
	return this->viewMatrix;
}



