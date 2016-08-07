/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "NormalMapShaderImplementation.h"

class NormalMapDeferredShader : public NormalMapShaderImplementation{

protected:
	std::string vertexShaderFilename;
	std::string fragmentShaderFilename;	

	// Permite a los hijos de esta clase agregar comportamiento al extendedDraw
	virtual void normalMapShaderChildsExtendedDraw(Mesh* mesh){};  

	// Permite a los hijos de esta clase agregar comportamiento al Init
	virtual void normalMapShaderChildsExtendedAttributesInit(list<string>* inputAttributes){};
	virtual void normalMapShaderChildsExtendedUniformsInit(){};

	void extendedDraw(Mesh* mesh){ this->normalMapShaderChildsExtendedDraw(mesh); }

public:
	NormalMapDeferredShader();
	void init();	
};