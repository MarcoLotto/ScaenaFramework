/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SceneLight.h"

// Clona la luz actual
SceneLight* SceneLight::clone(){
	SceneLight* clone = new SceneLight();
	clone->setAbsolutePosition(this->absolutePosition);
	clone->setActive(this->active);
	clone->setDirection(this->direction);
	clone->setIntensity(this->intensity);
	clone->setLightType(this->lightType);
	clone->setSpotFactor(this->spotFactor);
	this->getShadowData()->clone(clone->getShadowData());	
	return clone;
}