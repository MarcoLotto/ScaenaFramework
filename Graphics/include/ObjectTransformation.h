/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Transformation.h"
#include "ObjectLight.h"

// En si es lo mismo que Transformation, con la diferencia que el objeto precisa que al transformarse tambien
// se transformen sus luces attacheadas. Esta clase extiende el comportamiento para tambien actualizar las luces.
class ObjectTransformation : public Transformation{

private:
	vector<ObjectLight*>* objectAttachedLights;
	void updateObjectLightsPosition();
	void updateObjectLightsRotation();

public:
	ObjectTransformation(vector<ObjectLight*>* objectAttachedLights);

	void setPosition(vec3 position);
	void setRotation(vec3 rotation);
};
