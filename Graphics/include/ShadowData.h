/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <glm/glm.hpp> 
#include "CamaraPrimeraPersona.h"

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

// Define la informacion de una sombra para una luz
class ShadowData{

private:
	#define DEFAULT_SHADOW_SIZE 1024
	#define DEFAULT_FILTER_FACTOR 4.0f
	#define DEFAULT_PERSPECTIVE_FOVY 60.0f

	bool active;
	bool prebanked;
	bool generatedShadowMap;  //Indica si ya fue generado por lo menos una vez el shadow map
	vec2 shadowMapSize;
	float filterFactor;  // El factor de filtrado de la sombra (usualmente el factor de blur que se le aplica)
	float perspectiveFovy;  // La pesrpectiva con que se renderiza la sombra

	Camara* shadowCamera;  // La camara del punto de vista de la luz


public:
	ShadowData(){
		this->active = false;
		this->prebanked = false;
		this->generatedShadowMap = false;
		this->shadowMapSize = vec2(DEFAULT_SHADOW_SIZE);
		this->filterFactor = DEFAULT_FILTER_FACTOR;
		this->perspectiveFovy = DEFAULT_PERSPECTIVE_FOVY;
		this->shadowCamera = new CamaraPrimeraPersona();
	}
	virtual ~ShadowData(){
		delete this->shadowCamera;
	}

	bool isActive(){ return this->active; }
	void setActive(bool active){ this->active = active; }

	bool isPrebanked(){ return this->prebanked; }
	void setPrebanked(bool value){ this->prebanked = value; }

	// Indica si ya el shadow map para esta sombra esta generado (utilizado para prebanking)
	bool hasGeneratedShadowMap(){ return this->generatedShadowMap; }
	void setGeneratedShadowMap(bool isGenerated){ this->generatedShadowMap = isGenerated; }
	//Fuerza a que se genere el shadow map en el proximo ciclo de render (usar para shadowMaps prebankeados)
	void forceGenerationOfShadowMap(){ this->generatedShadowMap = false; }

	vec2 getShadowMapSize(){ return this->shadowMapSize; }
	void setShadowMapSize(vec2 sizeInPixels){ this->shadowMapSize = sizeInPixels; }

	float getFilterFactor(){ return this->filterFactor; }
	void setFilterFactor(bool value){ this->filterFactor = value; }

	float getPerspectiveFovy(){ return this->perspectiveFovy; }
	void setPerspectiveFovy(float value){ this->perspectiveFovy = value; }

	Camara* getShadowCamera(){ return this->shadowCamera; }

	void clone(ShadowData* cloneShadow){
		cloneShadow->setActive(this->isActive());
		cloneShadow->setFilterFactor(this->getFilterFactor());
		cloneShadow->setGeneratedShadowMap(this->hasGeneratedShadowMap());
		cloneShadow->setPerspectiveFovy(this->getPerspectiveFovy());
		cloneShadow->setPrebanked(this->isPrebanked());
		cloneShadow->setShadowMapSize(this->getShadowMapSize());
	}

};