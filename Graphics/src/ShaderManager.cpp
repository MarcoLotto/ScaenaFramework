/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "Mesh.h"
#include "ShaderManager.h"
#include "DifuseShader.h"
#include "NormalMapShader.h"
#include "ParallaxShader.h"
#include "SkinnedDifuseShader.h"
#include "SkinnedNormalMapShader.h"
#include "SkinnedParallaxShader.h"
#include "BlurShader.h"
#include "ShadowShader.h"

ShaderManager* ShaderManager::instance = NULL;

/********Cada vez que se crea un nuevo shader hay que declarlo******** 
*********en configure() y evaluarlo en evaluateTheBestShader()*******/

void ShaderManager::configure(){
	//Se agregan otros parametros de configuración
	this->lastPosition = vec3(0.0f, 0.0f, 0.0f);
	this->actualPosition = vec3(0.0f, 0.0f, 0.0f);
	this->isOptimizationNeeded = false;
	this->currentScene = NULL;
}

MeshDrawerShader* ShaderManager::reevaluateTheBestShader(Mesh* mesh){
	bool hasSkeleton = (mesh->getShaderProgram() == SkinnedDifuseShader::getInstance()) || 
		(mesh->getShaderProgram() == SkinnedNormalMapShader::getInstance()) ||
		(mesh->getShaderProgram() == SkinnedParallaxShader::getInstance());	
	return this->evaluateTheBestShader(mesh, hasSkeleton);
}

MeshDrawerShader* ShaderManager::evaluateTheBestShader(Mesh* mesh, bool hasSkeleton){
	//Aca configuro las reglas que mas se adaptan
	if(mesh->getMeshBuffers()->getVertexBuffer() != NULL){
		if(mesh->getDiffuseMap()->getMap() != NULL){	
			if(mesh->getNormalMap()->getMap() != NULL){						
				if(mesh->getDisplacementMap()->getMap() != NULL){
					if(hasSkeleton)
						return SkinnedDifuseShader::getInstance();
					return ParallaxShader::getInstance();
					}
					if(hasSkeleton)
						return SkinnedNormalMapShader::getInstance();
					return NormalMapShader::getInstance();					
			}
			if(hasSkeleton)
				return SkinnedDifuseShader::getInstance();
			return DifuseShader::getInstance();			
		}				
	}
	return NULL;
}

//**********************************************************************

ShaderManager* ShaderManager::getInstance(){
	if(instance == NULL){
		instance = new ShaderManager();
		instance->configure();
	}
	return instance;
}

void ShaderManager::initAllShaders(){
	DifuseShader::getInstance();
	NormalMapShader::getInstance();
	ParallaxShader::getInstance();
	SkinnedDifuseShader::getInstance();
	SkinnedNormalMapShader::getInstance();
	SkinnedParallaxShader::getInstance();
	ShadowShader::getInstance();
}

void ShaderManager::update(vec3 eyePosition){
	this->actualPosition = eyePosition;
	vec3 finalPosition = this->actualPosition - this->lastPosition;
	float deltaDistance = glm::length(finalPosition);
	if(deltaDistance >= DELTA_DISTANCE){
		this->lastPosition = this->actualPosition;
		this->isOptimizationNeeded = true;
	}
	else
		this->isOptimizationNeeded = false;
}

bool ShaderManager::optimizationsMustBeVerified(){
	return this->isOptimizationNeeded;
}

vec3 ShaderManager::getEyePoint(){
	return this->actualPosition;
}

void ShaderManager::useDeferredShadingStrategy(){
	// Seteo a cada shader que utilice la estrategia de deferred shading
	DifuseShader::getInstance()->useDeferredShadingImplementation();
	NormalMapShader::getInstance()->useDeferredShadingImplementation();
	ParallaxShader::getInstance()->useDeferredShadingImplementation();
	SkinnedDifuseShader::getInstance()->useDeferredShadingImplementation();
	SkinnedNormalMapShader::getInstance()->useDeferredShadingImplementation();
	SkinnedParallaxShader::getInstance()->useDeferredShadingImplementation();
}
void ShaderManager::useForwardShadingStrategy(){
	// Seteo a cada shader que utilice la estrategia de forward shading
	DifuseShader::getInstance()->useForwardShadingImplementation();
	NormalMapShader::getInstance()->useForwardShadingImplementation();
	ParallaxShader::getInstance()->useForwardShadingImplementation();
	SkinnedDifuseShader::getInstance()->useForwardShadingImplementation();
	SkinnedNormalMapShader::getInstance()->useForwardShadingImplementation();
	SkinnedParallaxShader::getInstance()->useForwardShadingImplementation();
}