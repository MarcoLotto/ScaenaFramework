/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ParallaxShaderImplementation.h"

class ParallaxDeferredShader : public ParallaxShaderImplementation{

protected:
	std::string vertexShaderFilename;
	std::string fragmentShaderFilename;	

	// Permite a los hijos de esta clase agregar comportamiento al extendedDraw
	virtual void parallaxShaderChildsExtendedDraw(Mesh* mesh){};  

	// Permite a los hijos de esta clase agregar comportamiento al Init
	virtual void parallaxShaderChildsExtendedAttributesInit(list<string>* inputAttributes){};
	virtual void parallaxShaderChildsExtendedUniformsInit(){};

	void extendedDraw(Mesh* mesh){ this->parallaxShaderChildsExtendedDraw(mesh); }

public:
	ParallaxDeferredShader();
	void init();	
};