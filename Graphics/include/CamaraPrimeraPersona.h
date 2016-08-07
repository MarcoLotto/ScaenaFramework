/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

#include "Camara.h"

//**********************************************************************************************************
//*											  CamaraPrimeraPersona
//-------------------------------------------------------------------------------------------------------
//* Con esta clase se puede instanciar una camara en primera persona. Se puede definir para la misma una
//* una posicion, rotaciones y angulos de rotacion máximos. Se debe llamar al método update en cada frame.
//* Para mover la camara se utiliza el moveFront, side y up. Rotaciones con setRotationPitch, Roll y Yaw.
//* Angulos de rotacion máximos con setMaxRotationAngles.
//**********************************************************************************************************
class CamaraPrimeraPersona : public Camara {

private:	

public:
	CamaraPrimeraPersona();
	void update();
	void moveFront(float velocity);
	void moveSide(float velocity);
	void moveUp(float velocity);
	void setEyePosition(vec3 position);
};