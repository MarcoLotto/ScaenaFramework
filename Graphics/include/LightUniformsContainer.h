/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "LightUniform.h"
#include <vector>
using namespace std;

class LightUniformsContainer{

private:
	vector<LightUniform*>* lightUniforms;
	GLSLUniform* activeLightCountUniform;

public:
	LightUniformsContainer(GLSLProgram* glslProgram);
	virtual ~LightUniformsContainer();

	vector<LightUniform*>* getLightUniforms(){ return this->lightUniforms; }
	void addLightUniform(LightUniform* lightUniform){ this->lightUniforms->push_back(lightUniform); }

	GLSLUniform* getActiveLightCountUniform(){ return this->activeLightCountUniform; }
};