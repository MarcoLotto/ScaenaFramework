/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "SceneLight.h"
#include "PropertyCannotBeAcessedException.h"

class ObjectLight : public SceneLight{

protected:
	vec3 localPosition;
	vec3 unrotatedDirection;

	vec3 objectPosition;
	vec3 objectRotation;

	void internalUpdateLightPositionAndDirection();

public:
	// La posicion local de la luz con respecto al objeto
	vec3 getLocalPosition(){ return this->localPosition; }
	void setLocalPosition(vec3 position);

	// Setea la direccion de la luz
	void setDirection(vec3 value);

	// Cuando el objecto cambia de posicion se debe mover tambien la luz
	void updateLightPositionAndDirection(vec3 objectPosition, vec3 objectRotation);

	// No deberia poder en este caso setear la posicion absoluta de la luz, ya que esto se calcula con la posicion local
	void setAbsolutePosition(vec3 position){ throw new PropertyCannotBeAcessedException("ObjectLight#setAbsolutePosition"); }

	// Clona la luz actual
	virtual SceneLight* clone();

	// No me termina de cerrar, pero muchas veces no encuentro otra forma de diferenciar sin gran impacto
	virtual string getClass(){ return "ObjectLight"; }
};
