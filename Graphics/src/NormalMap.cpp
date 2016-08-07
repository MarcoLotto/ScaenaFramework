/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "NormalMap.h"
#include "Mesh.h"

#define MAP_UNIFORM_NAME "normalMap"
#define FACTOR_UNIFORM_NAME "normalFactor"

NormalMap::NormalMap() : ChargeableTextureMap(){
	this->normalFactor = 1.0f;

	// Identifico los shaders
	this->mapUniform = new GLSLUniform(MAP_UNIFORM_NAME);
	this->factorUniform = new GLSLUniform(FACTOR_UNIFORM_NAME);	
}

NormalMap::~NormalMap(){
	delete this->mapUniform;
	delete this->factorUniform;
}

void NormalMap::chargeIntoShader(GLSLProgram* shader, unsigned int textureUnit){
	if(this->mapUniform->getGlslProgram() != shader){
		this->mapUniform->setGlslProgram(shader);
		this->factorUniform->setGlslProgram(shader);
	}
	this->mapTexture->assignToShader(shader, this->mapUniform, textureUnit);
	shader->setUniform(this->factorUniform, this->normalFactor);
}

void NormalMap::clone(Mesh* cloneMesh){
	NormalMap* clone = cloneMesh->getNormalMap();
	clone->setNormalFactor(this->normalFactor);
	if(this->mapTexture != NULL){
		clone->setMap(this->mapTexture->clone());
	}
}