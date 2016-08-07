/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "DifuseShaderImplementation.h"
#include "LightUniformsContainer.h"

class DifuseForwardShader : public DifuseShaderImplementation{

protected:
	std::string vertexShaderFilename;
	std::string fragmentShaderFilename;	

	// Permite a los hijos de esta clase agregar comportamiento al extendedDraw
	virtual void difuseShaderChildsExtendedDraw(Mesh* mesh){};  

	// Permite a los hijos de esta clase agregar comportamiento al Init
	virtual void difuseShaderChildsExtendedAttributesInit(list<string>* inputAttributes){};
	virtual void difuseShaderChildsExtendedUniformsInit(){}; 

	void extendedDraw(Mesh* mesh);

	GLSLUniform* vmMatrixUniform;
	GLSLUniform* gammaUniform;
	LightUniformsContainer* lightUniformsContainer;

public:
	void init();
};
