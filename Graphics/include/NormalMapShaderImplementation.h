/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshDrawerShader.h"

class NormalMapShaderImplementation : public MeshDrawerShader{

protected:
	void prepareForDraw(Mesh* mesh);
	void initOptimizationsParameters();	
	virtual void extendedDraw(Mesh* mesh) = 0;

	GLSLUniform* pvmMatrixUniform;
	GLSLUniform* normalMatrixUniform;
	GLSLUniform* decayFactorUniform;

	mat4 vmMatrix; // Se mantiene esta variable para evitar recalculo y mejorar performance

public:
	virtual void init() = 0;	
};