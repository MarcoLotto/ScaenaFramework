/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ObjectLight.h"

class Object;

class LightBuilder{

private:
	LightBuilder(){};
	static SceneLight* fillSceneLightDataStructure(vec3 pos, vec3 dir, vec3 intens, int type);
	static ObjectLight* fillObjectLightDataStructure(vec3 pos, vec3 dir, vec3 intens, int type);
	static vec3 normalizeWithZeroSafe(vec3 vectorToNormalize);

public:
	static SceneLight* buildPointSceneLight(vec3 position, vec3 intensity);
	static SceneLight* buildOnlyDiffusePointSceneLight(vec3 position, vec3 intensity);
	static SceneLight* buildDireccionalSceneLight(vec3 direction, vec3 intensity);
	static SceneLight* buildSpotSceneLight(vec3 position, vec3 direction, vec3 intensity, float factor);

	static ObjectLight* buildPointObjectLight(Object* ownerObject, vec3 localPosition, vec3 intensity);
	static ObjectLight* buildOnlyDiffusePointObjectLight(Object* ownerObject, vec3 localPosition, vec3 intensity);
	static ObjectLight* buildDireccionalObjectLight(Object* ownerObject, vec3 direction, vec3 intensity);
	static ObjectLight* buildSpotObjectLight(Object* ownerObject, vec3 localPosition, vec3 direction, vec3 intensity, float factor);
};