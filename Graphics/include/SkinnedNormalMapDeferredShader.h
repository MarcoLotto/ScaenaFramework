/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "NormalMapDeferredShader.h"

class SkinnedNormalMapDeferredShader : public NormalMapDeferredShader{

protected:
	GLSLUniform* jointsTransformUniform;
	
	// Permite a los hijos de esta clase agregar comportamiento al extendedDraw
	virtual void normalMapShaderChildsExtendedDraw(Mesh* mesh);  

	// Permite a los hijos de esta clase agregar comportamiento al Init
	virtual void difuseShaderChildsExtendedAttributesInit(list<string>* inputAttributes);
	virtual void difuseShaderChildsExtendedUniformsInit();

public:
	SkinnedNormalMapDeferredShader();
};