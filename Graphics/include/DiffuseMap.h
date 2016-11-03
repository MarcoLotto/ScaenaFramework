/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ChargeableTextureMap.h"

class Mesh;

class DiffuseMap : public ChargeableTextureMap{

private:
	GLSLUniform* mapUniform;

public:
	DiffuseMap();
	virtual ~DiffuseMap();

	void chargeIntoShader(GLSLProgram* shader, unsigned int textureUnit);
	void clone(Mesh* cloneMesh);
};