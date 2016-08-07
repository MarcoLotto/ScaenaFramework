/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ParallaxShaderImplementation.h"
#include "LightUniformsContainer.h"

class ParallaxForwardShader : public ParallaxShaderImplementation{

protected:
	std::string vertexShaderFilename;
	std::string fragmentShaderFilename;	

	// Permite a los hijos de esta clase agregar comportamiento al extendedDraw
	virtual void parallaxShaderChildsExtendedDraw(Mesh* mesh){};  

	// Permite a los hijos de esta clase agregar comportamiento al Init
	virtual void parallaxShaderChildsExtendedAttributesInit(list<string>* inputAttributes){};
	virtual void parallaxShaderChildsExtendedUniformsInit(){};

	void extendedDraw(Mesh* mesh);

	GLSLUniform* gammaUniform;
	LightUniformsContainer* lightUniformsContainer;

public:
	ParallaxForwardShader();
	void init();	
};