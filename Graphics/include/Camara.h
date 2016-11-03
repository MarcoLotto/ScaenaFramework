/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform2.hpp> 
#include <glm/gtx/projection.hpp>
#include "ViewFrustum.h"

typedef glm::mat4 mat4;
typedef glm::mat3 mat3;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::vec2 vec2;

//**********************************************************************************************************
//*											  Abstract : Camara
//-------------------------------------------------------------------------------------------------------
//* Define los elementos basicos que cualquier camara debe tener. De esta se heredan todos los subtipos.
//**********************************************************************************************************
class Camara {

protected:
	vec3 eyePosition;
	vec3 lookAt;
	vec3 upDireccion;
	vec3 sideDireccion;
	vec3 eyeRotation;
	vec3 maxRotAngles;
	vec3 baseRotationAngles;
	mat4 viewMatrix;
	mat4 perspectiveMatrix;
	ViewFrustum* viewFrustum;

public:
	Camara();
	virtual ~Camara();

	virtual void update() = 0;

	void configure();
	vec3 getEyePosition();		
	vec3 getLookAtPoint();
	void setUpDireccion(vec3 direccion);
	vec3 getUpDireccion();
	void setRotationPitch(float angle);
	void setRotationRoll(float angle);
	void setRotationYaw(float angle);

	//Setea angulos máximos a los que se tiene que restringir la rotacion de la cámara.
	//Estos angulos están definidos a partir de PitchRollYawBase y tiene una tolerancia de
	//PitchRollYawDelta. Si no se especifica, no hay ningún tipo de restricción.
	void setMaxRotationAngles(vec3 PitchRollYawBase, vec3 PitchRollYawDelta);	

	mat4 getViewMatrix();
	void setViewMatrix(mat4 matrix){ this->viewMatrix = matrix; }

	virtual float getEyeRotationPitch();	
	virtual float getEyeRotationYaw();
	virtual float getEyeRotationRoll();

	void setPerspective(float angle, float ratio, float nearD, float farD);
	mat4 getPerspectiveMatrix(){ return this->perspectiveMatrix; }

	ViewFrustum* getViewFrustum(){ return this->viewFrustum; }
};