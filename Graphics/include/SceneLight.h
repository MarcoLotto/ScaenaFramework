/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ShadowData.h"
#include <string>
using namespace std;

// Tipos admitidos de luces
#define LIGHT_TYPE_POINT 0
#define LIGHT_TYPE_DIRECCIONAL 1
#define LIGHT_TYPE_SPOT 2
#define LIGHT_TYPE_POINT_DIFFUSE 3

// Define las propiedades de una luz propia del ambito de la escena
class SceneLight{

protected:
	bool active;
	vec3 absolutePosition;
	vec3 intensity;
	vec3 direction;
	int lightType;
	float spotFactor;
	ShadowData* shadowData;
	
public:
	SceneLight(){ this->shadowData = new ShadowData(); }
	virtual ~SceneLight(){ delete this->shadowData; }

	// Inidica si la luz esta activa
	bool isActive(){ return this->active; }
	void setActive(bool active){ this->active = active; }

	// La posicion absoluta en la escena de la luz
	virtual vec3 getAbsolutePosition(){ return this->absolutePosition; }
	virtual void setAbsolutePosition(vec3 position){ this->absolutePosition = position; }

	// La intensidad en RGB de la luz
	vec3 getIntensity(){ return this->intensity; }
	void setIntensity(vec3 value){ this->intensity = value; }

	// La direccion de la luz (no tiene efecto si la luz es puntual)
	vec3 getDirection(){ return this->direction; }
	virtual void setDirection(vec3 value){ this->direction = value; }
	
	// El tipo de luz, puede ser spot, puntual, direccional o puntual solo difusa
	int getLightType(){ return this->lightType; }
	void setLightType(int type){ this->lightType = type; }
	
	// El factor de spot, esto afecta a que tan grande es el area afectada cuando se tiene una luz tipo spot
	float getSpotFactor(){ return this->spotFactor; }
	void setSpotFactor(float value){ this->spotFactor = value; }
	
	// La informacion de la sombra asociada
	ShadowData* getShadowData(){ return this->shadowData; }

	// Clona la luz actual
	virtual SceneLight* clone();

	// No me termina de cerrar, pero muchas veces no encuentro otra forma de diferenciar sin gran impacto
	virtual string getClass(){ return "SceneLight"; }
};

