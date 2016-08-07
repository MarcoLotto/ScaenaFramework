/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Object.h"

// Permite realizar movimientos de yaw-pitch-roll a un objeto (ej. moverse en una determinada direccion y que esta cambie a memida que rota)
class DirectionalMovingObject{

private:
	Object* object;

	// Se definen los tres vectores paar representar al objeto
	vec3 lookAt;
	vec3 upDirection;
	vec3 sideDirection;

public:
	// El objeto en cuestión y el ajuste respecto de la rotacion original del objeto para que coincidan los ejes de yaw-pitch-roll)
	DirectionalMovingObject(Object* object, vec3 rotationToAjustAxis);

	// Accesors para el objeto al que se le aplican las transformaciones
	void setObject(Object* object){ this->object = object; }
	Object* getObject(){ return this->object; }

	// Rotaciones en cada eje del objeto
	void rotate(vec3 rotationDegrees);
	
	// Translación en cada eje del objeto
	void goForward(float meters);
	void goBackward(float meters);
	void goLeft(float meters);
	void goRight(float meters);
	void goUp(float meters);
	void goDown(float meters);	
};