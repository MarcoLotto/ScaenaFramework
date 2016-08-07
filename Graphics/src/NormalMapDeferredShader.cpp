/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "NormalMapDeferredShader.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/NormalMapDeferredShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/NormalMapDeferredShader.frag"
//********************************************************************************************************

NormalMapDeferredShader::NormalMapDeferredShader(){
	this->initOptimizationsParameters();
}

void NormalMapDeferredShader::init(){
	this->vertexShaderFilename = VSHADER;
	this->fragmentShaderFilename = FSHADER;

	list<string> inputAttributes;
	inputAttributes.push_back("VertexPosition");
	inputAttributes.push_back("VertexNormal");
	inputAttributes.push_back("VertexTexCoord");
	inputAttributes.push_back("VertexTangent");
	inputAttributes.push_back("VertexBinormal");

	// Permito a los hijos de esta clase agregar mas atributos de entrada o salida
	this->normalMapShaderChildsExtendedAttributesInit(&inputAttributes);
	
	list<string> outputAttributes;
	outputAttributes.push_back("NormalData");
	outputAttributes.push_back("ColorData");
	outputAttributes.push_back("MaterialData");

	this->setVertexShaderName(this->vertexShaderFilename);
	this->setFragmentShaderName(this->fragmentShaderFilename);
	this->initialize(&inputAttributes, &outputAttributes); 

	// Permito a los hijos de esta clase agregar mas uniforms y otros componentes de inicializacion
	this->normalMapShaderChildsExtendedUniformsInit();

	// Identifico los uniforms a utilizar
	this->pvmMatrixUniform = new GLSLUniform("PVMmatrix", this);
	this->normalMatrixUniform = new GLSLUniform("normalMatrix", this);
	this->decayFactorUniform = new GLSLUniform("decayFactor", this);
}