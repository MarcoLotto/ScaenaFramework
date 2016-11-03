/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshDrawerShader.h"

class DifuseShaderImplementation : public MeshDrawerShader{

protected:
	void prepareForDraw(Mesh* mesh);
	virtual void extendedDraw(Mesh* mesh) = 0;

	GLSLUniform* pvmMatrixUniform;
	GLSLUniform* normalMatrixUniform;
	GLSLUniform* diffuseMapUniform;

	mat4 vmMatrix; // Se mantiene esta variable para evitar recalculo y mejorar performance

public:
	virtual void init() = 0;
};
