/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshDrawerShader.h"

class ParallaxShaderImplementation : public MeshDrawerShader{

protected:
	void prepareForDraw(Mesh* mesh);
	void initOptimizationsParameters();	
	virtual void extendedDraw(Mesh* mesh) = 0;

	GLSLUniform* pvmMatrixUniform;
	GLSLUniform* normalMatrixUniform;
	GLSLUniform* atenuationFactorUniform;
	GLSLUniform* vmMatrixUniform;

public:
	virtual void init() = 0;	
};