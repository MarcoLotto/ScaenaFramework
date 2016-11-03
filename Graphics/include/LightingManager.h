/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <list>
#include "GLSLProgram.h"
#include "SceneLight.h"
#include "ShadowManager.h"
#include "LightUniformsContainer.h"

class ObjectManager;

typedef glm::vec3 vec3;
typedef glm::vec2 vec2;
typedef glm::mat4 mat4;
typedef glm::mat3 mat3;

class LightingManager{

private:
	list<SceneLight*>* lightList;
	ShadowManager* shadowManager;
	
	vec3 normalizeWithZeroSafe(vec3 vectorToNormalize);
	bool isLightAlreadyLoaded(SceneLight* light);  // Identifica si una luz ya esta cargada en el manager
	
public:	
	LightingManager(ObjectManager* objectManager);
	virtual ~LightingManager();

	//Crea luces de los 3 tipos (mas una luz puntual solo difusa, mas rápida que la común para simular iluminación global)
	//La luz es devuelta y agregada en el manager
	SceneLight* setPointLight(vec3 posicion, vec3 intensidad);
	SceneLight* setPointLightOnlyDiffuse(vec3 posicion, vec3 intensidad);
	SceneLight* setDireccionalLight(vec3 direccion, vec3 intensidad);
	SceneLight* setSpotLight(vec3 posicion, vec3 direccion, vec3 intensidad, float factor);
	
	// Agrega una luz al manager (si no esta ya cargada). Devuelve el identificador o handler de la luz en el manager.
	void addLight(SceneLight* light);

	// Devuelve todas las luces cargadas en el manager
	list<SceneLight*>* getLights(){ return this->lightList; }

	//Indica si la luz castea sombra
	void castShadow(bool value, SceneLight* light);

	//Remueve (pero no borra fisicamente) una luz del manager (si existe)
	void removeLight(SceneLight* light);

	// Remueve todas las luces del manager (pero no las borra fisicamente)
	void clear();

	//Borra (fisicamente) todas las luces del sistema
	void deleteAllLights();

	//Se llama en el render de cada objeto
	void renderLights(GLSLProgram* shaderProgram, LightUniformsContainer* lightUniformsContainer, mat4 viewMatrix, unsigned int textureUnit);

	// Clona al manager junto con todas sus luces. Debe indicarse el objectmanager (o NULL pero hay que setearselo a mano al shadowManager luego)
	LightingManager* clone(ObjectManager* objectManager);
	
	// Devuelve el shadow manager, encargado de manejar todas las sombras de las luces
	ShadowManager* getShadowManager(){ return this->shadowManager; }
};