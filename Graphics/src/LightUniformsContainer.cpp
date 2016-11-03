/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "LightUniformsContainer.h"

#define ACTIVE_LIGHT_COUNT_UNIFORM_NAME "cantLucesActivas"

LightUniformsContainer::LightUniformsContainer(GLSLProgram* glslProgram){
	this->lightUniforms = new vector<LightUniform*>();
	this->activeLightCountUniform = new GLSLUniform(ACTIVE_LIGHT_COUNT_UNIFORM_NAME, glslProgram);
}

LightUniformsContainer::~LightUniformsContainer(){
	// Borro cada una de las light uniform y el contenedor
	vector<LightUniform*>::iterator it = this->lightUniforms->begin();
	while( it != this->lightUniforms->end() ){
		delete (*it);
		it = this->lightUniforms->erase(it);
	}
	delete this->lightUniforms;

	// Borro el uniform de cantidad de luces activas
	delete this->activeLightCountUniform;
}