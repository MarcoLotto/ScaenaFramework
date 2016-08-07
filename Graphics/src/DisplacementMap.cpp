/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "DisplacementMap.h"
#include "Mesh.h"

#define MAP_UNIFORM_NAME "displacementMap"
#define FACTOR_UNIFORM_NAME "displacementFactor"

DisplacementMap::DisplacementMap() : ChargeableTextureMap(){
	this->displacementFactor = 1.0f;

	// Identifico los shaders
	this->mapUniform = new GLSLUniform(MAP_UNIFORM_NAME);
	this->factorUniform = new GLSLUniform(FACTOR_UNIFORM_NAME);
}

DisplacementMap::~DisplacementMap(){
	delete this->mapUniform;
	delete this->factorUniform;
}

void DisplacementMap::chargeIntoShader(GLSLProgram* shader, unsigned int textureUnit){
	if(this->mapUniform->getGlslProgram() != shader){
		this->mapUniform->setGlslProgram(shader);
		this->factorUniform->setGlslProgram(shader);
	}
	this->mapTexture->assignToShader(shader, this->mapUniform, textureUnit);
	shader->setUniform(this->factorUniform, this->displacementFactor);
}

void DisplacementMap::clone(Mesh* cloneMesh){
	DisplacementMap* clone = cloneMesh->getDisplacementMap();
	clone->setDisplacementFactor(this->displacementFactor);
	if(this->mapTexture != NULL){
		clone->setMap(this->mapTexture->clone());
	}
}