/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "DiffuseMap.h"
#include "Mesh.h"

#define MAP_UNIFORM_NAME "diffuseMap"

DiffuseMap::DiffuseMap(){
	// Identifico los shaders
	this->mapUniform = new GLSLUniform(MAP_UNIFORM_NAME);
}
DiffuseMap::~DiffuseMap(){
	delete this->mapUniform;
}

void DiffuseMap::chargeIntoShader(GLSLProgram* shader, unsigned int textureUnit){
	if(this->mapUniform->getGlslProgram() != shader){
		this->mapUniform->setGlslProgram(shader);
	}
	this->mapTexture->assignToShader(shader, this->mapUniform, textureUnit);
}
void DiffuseMap::clone(Mesh* cloneMesh){
	DiffuseMap* clone = cloneMesh->getDiffuseMap();
	if(this->mapTexture != NULL){
		clone->setMap(this->mapTexture->clone());
	}
}