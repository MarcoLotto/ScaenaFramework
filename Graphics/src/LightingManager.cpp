/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include <glm/glm.hpp> 
#include "LightingManager.h"
#include "LightDoesNotExistException.h"
#include "ObjectManager.h"
#include "LightBuilder.h"

LightingManager::LightingManager(ObjectManager* objectManager){
	this->lightList = new list<SceneLight*>();
	this->shadowManager = new ShadowManager(objectManager);

	// Registro en el object manager este lightManager
	if(objectManager != NULL)
		objectManager->setLightingManager(this);
}

//********************************Creacion**************************************
SceneLight* LightingManager::setPointLight(vec3 posicion, vec3 intensidad){
	SceneLight* light = LightBuilder::buildPointSceneLight(posicion, intensidad);
	this->addLight(light);
	return light;
}
SceneLight* LightingManager::setPointLightOnlyDiffuse(vec3 posicion, vec3 intensidad){
	SceneLight* light = LightBuilder::buildOnlyDiffusePointSceneLight(posicion, intensidad);
	this->addLight(light);
	return light;
}
SceneLight* LightingManager::setDireccionalLight(vec3 direccion, vec3 intensidad){
	SceneLight* light = LightBuilder::buildDireccionalSceneLight(direccion, intensidad);
	this->addLight(light);
	return light;
}
SceneLight* LightingManager::setSpotLight(vec3 posicion, vec3 direccion, vec3 intensidad, float factor){
	SceneLight* light = LightBuilder::buildSpotSceneLight(posicion, direccion, intensidad, factor);
	this->addLight(light);
	return light;
}

//**********************************Borrado*****************************************
void LightingManager::removeLight(SceneLight* light){
	//Busco la luz, la saco de la lista del manager y la borro de la memoria
	list<SceneLight*>::iterator it = this->lightList->begin();
	for(; it != this->lightList->end(); it++){
		if((*it) == light){
			this->lightList->erase(it);
			return;
		}
	}
}
void LightingManager::deleteAllLights(){
	//Borro todas las luces y la lista de luces
	list<SceneLight*>::iterator it = this->lightList->begin();
	for(; it != this->lightList->end(); it++){
		delete (*it);
	}
	this->lightList->clear();
}

void LightingManager::clear(){
	this->lightList->clear();
}
//**********************************Render*****************************************
void LightingManager::renderLights(GLSLProgram* shaderProgram, LightUniformsContainer* lightUniformsContainer, mat4 viewMatrix, unsigned int textureUnit){

	// Agrego si hace falta, todos los uniforms necesarios
	for(unsigned int i = lightUniformsContainer->getLightUniforms()->size(); i < this->lightList->size(); i++){
		lightUniformsContainer->addLightUniform(new LightUniform(i, shaderProgram));
	}

	int shadowIndex = 0;  //Determina la posicion en donde se guardan los shadowMaps
	char* nombreUniform = new char[30]();
	vec4 posicionFinal;
	int lightIndex=0;
	list<SceneLight*>::iterator it = this->lightList->begin();
	for(; it != this->lightList->end(); it++){
		SceneLight* light = (*it);
		if(light->isActive()){	
			LightUniform* lightUniform = lightUniformsContainer->getLightUniforms()->at(lightIndex);

			//Asigno el tipo de luz como la cuarta coordenada de la posicion
			posicionFinal = viewMatrix * vec4(light->getAbsolutePosition(), 1.0f);
			posicionFinal.w = (float) light->getLightType();			
			shaderProgram->setUniform(lightUniform->getPositionUniform(), 1, posicionFinal);					
			shaderProgram->setUniform(lightUniform->getDirectionUniform(), 1, vec3(mat3(viewMatrix) * light->getDirection()));						
			shaderProgram->setUniform(lightUniform->getIntensityUniform(), light->getIntensity());						
			shaderProgram->setUniform(lightUniform->getSpotFactorUniform(), light->getSpotFactor());			
	
			//Si es necesario, genero sombras para la luz y asigno al shader
			if((light->getShadowData()->isActive())&&(shadowIndex < shadowManager->getMaxShadowMapCount())){	
				//Asigno el shadowMap a la segunda pasada del shader				
				Texture* shadowMap = shadowManager->getShadowMap(light);
				shadowMap->assignToShader(shaderProgram, lightUniform->getShadowMapUniform(), textureUnit + shadowIndex);
				//Asigno la shadowMatrix				
				mat4 shadowMatrix = shadowManager->getShadowMatrix(light) * glm::inverse(viewMatrix);
				shaderProgram->setUniform(lightUniform->getShadowMatrixUniform(), shadowMatrix);
				//Seteo el indice correspondiente					
				shaderProgram->setUniform(lightUniform->getShadowIndexUniform(), shadowIndex);
				shadowIndex++;
			}
			else{		
				//Seteo -1 en el shadow index indicando que no hay sombra asignada						
				shaderProgram->setUniform(lightUniform->getShadowIndexUniform(), -1);
			}
			//Aumento el lightindex
			lightIndex++;			
		}
	}
	shaderProgram->setUniform(lightUniformsContainer->getActiveLightCountUniform(), lightIndex);
	delete nombreUniform;	
}

void LightingManager::addLight(SceneLight* light){
	if(!this->isLightAlreadyLoaded(light))
		this->lightList->push_back(light);	
}

//**********************************Helpers*****************************************
bool LightingManager::isLightAlreadyLoaded(SceneLight* light){
	list<SceneLight*>::iterator it = this->lightList->begin();
	for(; it != this->lightList->end(); it++){
		if(light == (*it))
			return true;
	}
	return false;
}

//**********************************Sombras*****************************************
void LightingManager::castShadow(bool value, SceneLight* light){
	ShadowData* shadow = light->getShadowData();
	shadow->setActive(value);
	vec2 shadowMapSize = shadow->getShadowMapSize();
	if(value){
		//Creo un nuevo shadow map para esa luz, si ya existe no hago nada
		shadowManager->createNewShadowMap(light, shadowMapSize);
	}
	else{
		//Si la luz tiene asociado un shadow map lo borra, sino no hace nada
		shadowManager->destroyShadowMap(light);
	}
}

LightingManager* LightingManager::clone(ObjectManager* objectManager){
	LightingManager* clone = new LightingManager(objectManager);
	list<SceneLight*>::iterator it = this->lightList->begin();
	while(it != this->lightList->end()){
		SceneLight* cloneLight = (*it)->clone();
		clone->addLight(cloneLight);
		++it;
	}
	return clone;
}

LightingManager::~LightingManager(){
	if(this->shadowManager != NULL){
		delete this->shadowManager;
		this->shadowManager = NULL;
	}
	this->deleteAllLights();
	delete this->lightList;	
}

