/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "LightBuilder.h"
#include "Object.h"

SceneLight* LightBuilder::buildPointSceneLight(vec3 position, vec3 intensity){
	SceneLight* light = fillSceneLightDataStructure(position, vec3(0.0f, 0.0f, 0.0f), intensity, LIGHT_TYPE_POINT);
	light->setSpotFactor(0.0f);
	return light;
}
SceneLight* LightBuilder::buildOnlyDiffusePointSceneLight(vec3 position, vec3 intensity){
	SceneLight* light = fillSceneLightDataStructure(position, vec3(0.0f, 0.0f, 0.0f), intensity, LIGHT_TYPE_POINT_DIFFUSE);
	light->setSpotFactor(0.0f);
	return light;
}
SceneLight* LightBuilder::buildDireccionalSceneLight(vec3 direction, vec3 intensity){
	SceneLight* light = fillSceneLightDataStructure(vec3(0.0f, 0.0f, 0.0f), direction, intensity, LIGHT_TYPE_DIRECCIONAL);
	light->setSpotFactor(0.0f);
	return light;
}
SceneLight* LightBuilder::buildSpotSceneLight(vec3 position, vec3 direction, vec3 intensity, float factor){
	SceneLight* light = fillSceneLightDataStructure(position, direction, intensity, LIGHT_TYPE_SPOT);
	light->setSpotFactor(factor);
	return light;
}

ObjectLight* LightBuilder::buildPointObjectLight(Object* ownerObject, vec3 localPosition, vec3 intensity){
	ObjectLight* light = fillObjectLightDataStructure(localPosition, vec3(0.0f, 0.0f, 0.0f), intensity, LIGHT_TYPE_POINT);
	light->setSpotFactor(0.0f);
	light->updateLightPositionAndDirection(ownerObject->getTransformation()->getPosition(), ownerObject->getTransformation()->getRotation());
	return light;
}
ObjectLight* LightBuilder::buildOnlyDiffusePointObjectLight(Object* ownerObject, vec3 localPosition, vec3 intensity){
	ObjectLight* light = fillObjectLightDataStructure(localPosition, vec3(0.0f, 0.0f, 0.0f), intensity, LIGHT_TYPE_POINT_DIFFUSE);
	light->setSpotFactor(0.0f);
	light->updateLightPositionAndDirection(ownerObject->getTransformation()->getPosition(), ownerObject->getTransformation()->getRotation());
	return light;
}
ObjectLight* LightBuilder::buildDireccionalObjectLight(Object* ownerObject, vec3 direction, vec3 intensity){
	ObjectLight* light = fillObjectLightDataStructure(vec3(0.0f, 0.0f, 0.0f), direction, intensity, LIGHT_TYPE_DIRECCIONAL);
	light->setSpotFactor(0.0f);
	light->updateLightPositionAndDirection(ownerObject->getTransformation()->getPosition(), ownerObject->getTransformation()->getRotation());
	return light;
}
ObjectLight* LightBuilder::buildSpotObjectLight(Object* ownerObject, vec3 localPosition, vec3 direction, vec3 intensity, float factor){
	ObjectLight* light = fillObjectLightDataStructure(localPosition, direction, intensity, LIGHT_TYPE_SPOT);
	light->setSpotFactor(factor);
	light->updateLightPositionAndDirection(ownerObject->getTransformation()->getPosition(), ownerObject->getTransformation()->getRotation());
	return light;
}

SceneLight* LightBuilder::fillSceneLightDataStructure(vec3 pos, vec3 dir, vec3 intens, int type){
	SceneLight* light = new SceneLight();
	light->setAbsolutePosition(pos);
	light->setDirection(normalizeWithZeroSafe(dir));
	light->setIntensity(intens);
	light->setLightType(type);
	light->setActive(true);
	return light;
}

ObjectLight* LightBuilder::fillObjectLightDataStructure(vec3 localPos, vec3 dir, vec3 intens, int type){
	ObjectLight* light = new ObjectLight();
	light->setLocalPosition(localPos);
	light->setDirection(normalizeWithZeroSafe(dir));
	light->setIntensity(intens);
	light->setLightType(type);
	light->setActive(true);
	return light;
}

vec3 LightBuilder::normalizeWithZeroSafe(vec3 vectorToNormalize){
	if(vectorToNormalize == vec3(0.0f)) 
		return vectorToNormalize;
	return glm::normalize(vectorToNormalize);
}