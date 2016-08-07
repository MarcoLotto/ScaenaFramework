/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ChargeableTextureMap.h"

class Mesh;

class DisplacementMap : public ChargeableTextureMap{

private:
	float displacementFactor;

	GLSLUniform* mapUniform;
	GLSLUniform* factorUniform;

public:
	DisplacementMap();
	virtual ~DisplacementMap();

	void chargeIntoShader(GLSLProgram* shader, unsigned int textureUnit);
	void clone(Mesh* cloneMesh);

	float getDisplacementFactor(){ return this->displacementFactor; }
	void setDisplacementFactor(float value){ this->displacementFactor = value; }
};