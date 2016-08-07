/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ChargeableTextureMap.h"

class Mesh;

class NormalMap : public ChargeableTextureMap{

private:
	float normalFactor;

	GLSLUniform* mapUniform;
	GLSLUniform* factorUniform;

public:
	NormalMap();
	virtual ~NormalMap();

	void chargeIntoShader(GLSLProgram* shader, unsigned int textureUnit);
	void clone(Mesh* cloneMesh);

	float getNormalFactor(){ return this->normalFactor; }
	void setNormalFactor(float value){ this->normalFactor = value; }
};