/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GLSLProgram.h"

class Mesh;

// Lo implementan todos los recursos de Mesh que tienen algún uniform que cargar en el shader
class ChargeableResource{

public:
	virtual void chargeIntoShader(GLSLProgram* shader, unsigned int textureUnit) = 0;
	virtual void clone(Mesh* cloneMesh) = 0;
};