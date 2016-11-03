/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

#include "Camara.h"

//**********************************************************************************************************
//*											  CamaraTerceraPersona
//-------------------------------------------------------------------------------------------------------
//* Con esta clase se puede instanciar una camara en tercera persona. Se puede definir para la misma
//* un lookAtPoint, distancia al mismo, rotaciones y angulos de rotacion máximos. Se debe llamar al método 
//* update en cada frame. Para mover la camara se debe modificar el lookAtPoint y/o la distancia al mismo.
//* Rotaciones con setRotationPitch, Roll y Yaw. Angulos de rotacion máximos con setMaxRotationAngles.
//**********************************************************************************************************
class CamaraTerceraPersona : public Camara {

private:
	float distanceToPoint;
	float zoomAmout;
	float zoomFocusTime;
	float zoomTotalTime;
	int zoomTimeSlot;

	// El ojo tiene limitaciones al ser rotado, pero utilizando esto no se aplican las restricciones
	vec3 externalEyePointRotation;

	void updateDirectionVectors(vec3 lookAtDirection);
	float rotationAngleBetweenVectors(vec3 u, vec3 v);
	float norm(vec3 v);
	float getUpdateDistanceToPoint();


public:
	CamaraTerceraPersona();
	virtual ~CamaraTerceraPersona();

	void update();
	void setDistanceToPoint(float distance);
	float getDistanceToPoint();
	void setLookAtPoint(vec3 lookAt);

	// Rota el ojo, sin tener en cuenta las restricciones de angulo de la camara
	void rotateEyePoint(vec3 rotation);

	virtual float getEyeRotationPitch();
	virtual float getEyeRotationYaw();
	virtual float getEyeRotationRoll();

	// Aplica un zoom a la camara actual el cual estara determinado por el zoomAmount, y tardara en llegar a ese zoom una cantidad focusTime... El zoom durara zoomTime...
	void makeZoom(float zoomAmount, int focusTimeMilliseconds, int zoomTimeMilliseconds);  
};