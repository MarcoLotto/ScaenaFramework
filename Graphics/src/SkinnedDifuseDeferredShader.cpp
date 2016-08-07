/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SkinnedDifuseDeferredShader.h"
#include "SkinnedDrawerHelper.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/SkinnedDifuseDeferredShader.vert"
//********************************************************************************************************

void SkinnedDifuseDeferredShader::difuseShaderChildsExtendedDraw(Mesh* mesh){
	SkinnedDrawerHelper::skinDraw(this, mesh, this->jointsTransformUniform);
}

void SkinnedDifuseDeferredShader::difuseShaderChildsExtendedAttributesInit(list<string>* inputAttributes){
	this->vertexShaderFilename = VSHADER;

	inputAttributes->push_back("VertexTangent");  
	inputAttributes->push_back("VertexBinormal"); 
	SkinnedDrawerHelper::skinAttributesInit(this, inputAttributes);
}

void SkinnedDifuseDeferredShader::difuseShaderChildsExtendedUniformsInit(){
	this->vertexShaderFilename = VSHADER;
	SkinnedDrawerHelper::skinUniformsInit(this, &this->jointsTransformUniform);
}