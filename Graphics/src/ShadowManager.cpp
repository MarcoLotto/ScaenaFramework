/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ShadowManager.h"
#include "GraphicDevice.h"
#include "VarianceShadowMapStage.h"
#include "ShadowBlurStage.h"
#include "RenderPipelineWithGeometryAndBackWrite.h"
#include "CamaraPrimeraPersona.h"

//Temporal hasta hacer configuraciones de verdad
#include "VideoConfiguration.h"

//Define el numero maximo de shadow maps disponibles (cambiar tambien en GLSL)
#define MAX_SHADOWMAP_COUNT 3

//Defino el size y escala del poligon offset
#define POLIGON_OFFSET_FACTOR 0.00001f
#define POLIGON_OFFSET_SCALE 2.0f

#define PI 3.1415927f

ShadowManager::ShadowManager(ObjectManager* objectManager){
	this->renderPipeline = NULL;
	this->createBiasMatrix();
	this->objectManager = objectManager;
}

void ShadowManager::createNewShadowMap(SceneLight* light, vec2 shadowMapSize){
	if(getShadowMap(light) == NULL){
		//El shadowMap stage no existe, lo genero
		VarianceShadowMapStage* shadowStage = this->generateShadowMap(light, shadowMapSize.x, shadowMapSize.y);

		// Genero la matriz de perspectiva de la sombra de la luz y actualizo la vista
		this->generateShadowPerspective(light);
		this->updateLightShadowView(light);

		//Inserto el shadowMap stage en el mapa (lo vinculo con la luz)
		this->activeShadowMaps.insert(std::pair<SceneLight*, VarianceShadowMapStage*>(light, shadowStage));
	}
}

Texture* ShadowManager::getShadowMap(SceneLight* light){
	map<SceneLight*, VarianceShadowMapStage*>::iterator it = this->activeShadowMaps.find(light);	
	if(it != this->activeShadowMaps.end())
	{
		//Luz encontrada, devuelvo el shadowMap
		VarianceShadowMapStage* shadowStage = (it->second);
		if(shadowStage->getFilter() != NULL){
			// Si tiene un filtro activo, devuelvo la textura filtrada
			return shadowStage->getFilter()->getBackBuffer()->getOutputTexture();
		}
		// Si no tiene filtro, devuelvo la textura directa del shadow map
		return shadowStage->getBackBuffer()->getOutputTexture();	
	}
	//Elemento no encontrado
	return NULL;
}

void ShadowManager::updateShadowFilterFactor(SceneLight* light){
	map<SceneLight*, VarianceShadowMapStage*>::iterator it = this->activeShadowMaps.find(light);	
	if(it != this->activeShadowMaps.end())
	{
		//Luz encontrada, verifico si tiene filtro activo y cambio su factor de blur
		BackBufferWriterStage* filterStage = (it->second)->getFilter();
		if(filterStage != NULL){
			// REVIEW: No esta bien castear asi, encontrar otra forma de hacer esto 
			((ShadowBlurStage*) filterStage)->changeBlurFactor(light->getShadowData()->getFilterFactor());
		}
	}
}

void ShadowManager::destroyShadowMap(SceneLight* light){
	map<SceneLight*, VarianceShadowMapStage*>::iterator it = this->activeShadowMaps.find(light);	
	if(it != this->activeShadowMaps.end())
	{
		// Borro el stage del pipeline
		VarianceShadowMapStage* stage = (it->second);
		// Si tiene asociado un filtro lo borro tambien
		if(stage->getFilter() != NULL){
			if(this->renderPipeline != NULL){
				this->renderPipeline->removeBeforeGeometry(stage->getFilter());
			}
		}		
		// Borro el stage del pipeline y lo recargo
		if(this->renderPipeline != NULL){
			this->renderPipeline->removeBeforeGeometry(stage);
			this->renderPipeline->updatePipelineScheme();
		}
		if(stage->getFilter() != NULL){
			delete stage->getFilter();
		}
		delete stage;
		
		// Borro del mapa el shadow map
		this->activeShadowMaps.erase(it);
	}
}

// Agrega un nuevo stage en el pipeline (para un variance shadow map en principio)
VarianceShadowMapStage* ShadowManager::generateShadowMap(SceneLight* light, int width, int height){
	// Una primer etapa de creacion del variance shadow map
	VarianceShadowMapStage* shadowMapStage = new VarianceShadowMapStage(light, width, height);
	shadowMapStage->setObjectManager(this->objectManager);
	shadowMapStage->setShadowManager(this);

	// Una segunda etapa de blur
	ShadowBlurStage* blurStage = new ShadowBlurStage(shadowMapStage, width, height, light->getShadowData()->getFilterFactor());
	shadowMapStage->setFilter(blurStage);

	// Si tengo un pipeline, actualizo el mismo
	if(this->renderPipeline != NULL){
		this->renderPipeline->addBeforeGeometry(shadowMapStage);
		this->renderPipeline->addBeforeGeometry(blurStage);
		this->renderPipeline->updatePipelineScheme();
	}
	return shadowMapStage;
}

float ShadowManager::norm(vec3 v){
	return glm::sqrt((v.x)*(v.x)+ (v.y)*(v.y) + (v.z)*(v.z));
}

float ShadowManager::rotationAngleBetweenVectors(vec3 u, vec3 v){
	float angle = glm::dot(u, v) / (norm(u) * norm(v));
	angle = glm::acos(angle);
	angle -= PI / 2.0f;  //los quiero perpendiculares
	return (angle/PI)*180.0f;
}

void ShadowManager::updateLightShadowView(SceneLight* light){
	vec3 up = vec3(0.0f, 0.0f, 1.0f);
	vec3 side = vec3(0.0f, 1.0f, 0.0f);
	float angleUp = rotationAngleBetweenVectors(light->getDirection(), up);
	float angleSide = rotationAngleBetweenVectors(light->getDirection(), side);
	mat4 rotMat = glm::rotate(mat4(1.0f), angleUp, vec3(0.0f, 1.0f, 0.0f));
	rotMat = glm::rotate(rotMat, angleSide, vec3(0.0f, 0.0f, 1.0f));
	up = vec3(rotMat * vec4(up, 1.0f));
	side = glm::cross(light->getDirection(), up);
	// Actualizo en la camara la matriz de vista y el viewing frustum
	light->getShadowData()->getShadowCamera()->setViewMatrix(glm::lookAt(vec3(light->getAbsolutePosition()), vec3(light->getAbsolutePosition()) + light->getDirection(), up));	
	light->getShadowData()->getShadowCamera()->getViewFrustum()->update(light->getAbsolutePosition(), light->getDirection(), up, side);
}

void ShadowManager::generateShadowPerspective(SceneLight* light){
	return light->getShadowData()->getShadowCamera()->setPerspective(light->getShadowData()->getPerspectiveFovy(), 1.2f, 0.1f, 1000.0f);  // REVIEW: Hardcode
}

mat4 ShadowManager::getShadowMatrix(SceneLight* light){
	Camara* shadowCamera = light->getShadowData()->getShadowCamera();
	return (this->Bmatrix * shadowCamera->getPerspectiveMatrix() * shadowCamera->getViewMatrix());
}

void ShadowManager::createBiasMatrix(){
		//Creo la Bias matrix para las sombras	
		this->Bmatrix[0][0] = 0.5f;
		this->Bmatrix[1][0] = 0.0f;
		this->Bmatrix[2][0] = 0.0f;
		this->Bmatrix[3][0] = 0.5f;
		this->Bmatrix[0][1] = 0.0f;
		this->Bmatrix[1][1] = 0.5f;
		this->Bmatrix[2][1] = 0.0f;
		this->Bmatrix[3][1] = 0.5f;
		this->Bmatrix[0][2] = 0.0f;
		this->Bmatrix[1][2] = 0.0f;
		this->Bmatrix[2][2] = 0.5f;
		this->Bmatrix[3][2] = 0.5f;
		this->Bmatrix[0][3] = 0.0f;
		this->Bmatrix[1][3] = 0.0f;
		this->Bmatrix[2][3] = 0.0f;
		this->Bmatrix[3][3] = 1.0f;
}

int ShadowManager::getMaxShadowMapCount(){
	return MAX_SHADOWMAP_COUNT;
}

void ShadowManager::setRenderPipeline(RenderPipelineWithGeometryAndBackWrite* pipeline){
	this->renderPipeline = pipeline;

	// Si hay shadow maps que debiesen estar activos, los cargo al pipeline
	std::map<SceneLight*, VarianceShadowMapStage*>::iterator it = this->activeShadowMaps.begin();
	while(it != this->activeShadowMaps.end()){
		VarianceShadowMapStage* shadowStage = (*it).second;
		this->renderPipeline->addBeforeGeometry(shadowStage);
		// Si el shadow map tiene un filtro asignado, lo agrego al pipeline
		if(shadowStage->getFilter() != NULL){
			this->renderPipeline->addBeforeGeometry(shadowStage->getFilter());
		}
		++it;
	}
	// Recargo el pipeline
	this->renderPipeline->updatePipelineScheme();
}