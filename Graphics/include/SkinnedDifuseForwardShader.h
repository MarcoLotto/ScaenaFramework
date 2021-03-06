/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "DifuseForwardShader.h"

class SkinnedDifuseForwardShader : public DifuseForwardShader{

protected:
	GLSLUniform* jointsTransformUniform;	
	
	// Permite a los hijos de esta clase agregar comportamiento al extendedDraw
	virtual void difuseShaderChildsExtendedDraw(Mesh* mesh);  

	// Permite a los hijos de esta clase agregar comportamiento al Init
	virtual void difuseShaderChildsExtendedAttributesInit(list<string>* inputAttributes);
	virtual void difuseShaderChildsExtendedUniformsInit();
};
