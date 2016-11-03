/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ParallaxDeferredShader.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/ParallaxDeferredShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/ParallaxDeferredShader.frag"
//********************************************************************************************************

ParallaxDeferredShader::ParallaxDeferredShader(){
	this->initOptimizationsParameters();
}

void ParallaxDeferredShader::init(){
	this->vertexShaderFilename = VSHADER;
	this->fragmentShaderFilename = FSHADER;

	list<string> inputAttributes;
	inputAttributes.push_back("VertexPosition");
	inputAttributes.push_back("VertexNormal");
	inputAttributes.push_back("VertexTexCoord");
	inputAttributes.push_back("VertexTangent");
	inputAttributes.push_back("VertexBinormal");

	// Permito a los hijos de esta clase agregar mas atributos de entrada o salida
	this->parallaxShaderChildsExtendedAttributesInit(&inputAttributes);
	
	list<string> outputAttributes;
	outputAttributes.push_back("NormalData");
	outputAttributes.push_back("ColorData");
	outputAttributes.push_back("MaterialData");

	this->setVertexShaderName(this->vertexShaderFilename);
	this->setFragmentShaderName(this->fragmentShaderFilename);
	this->initialize(&inputAttributes, &outputAttributes);

	// Permito a los hijos de esta clase agregar mas uniforms y otros componentes de inicializacion
	this->parallaxShaderChildsExtendedUniformsInit();

	// Identifico los uniforms a utilizar
	this->pvmMatrixUniform = new GLSLUniform("PVMmatrix", this);
	this->normalMatrixUniform = new GLSLUniform("normalMatrix", this);
	this->atenuationFactorUniform = new GLSLUniform("atenuationFactor", this);
	this->vmMatrixUniform = new GLSLUniform("VMmatrix", this);	
}