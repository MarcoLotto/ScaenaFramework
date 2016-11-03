/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SkinnedNormalMapForwardShader.h"
#include "SkinnedDrawerHelper.h"
#include "SkinnedDifuseShader.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/SkinnedNormalMapForwardShader.vert"
//********************************************************************************************************

SkinnedNormalMapForwardShader::SkinnedNormalMapForwardShader(){ 
	this->initOptimizationsParameters(); 
	this->setLowerShader(SkinnedDifuseShader::getInstance()); //A que shader pasa al optimizar
}

void SkinnedNormalMapForwardShader::normalMapShaderChildsExtendedDraw(Mesh* mesh){
	SkinnedDrawerHelper::skinDraw(this, mesh, this->jointsTransformUniform);
}

void SkinnedNormalMapForwardShader::difuseShaderChildsExtendedAttributesInit(list<string>* inputAttributes){
	this->vertexShaderFilename = VSHADER;
	SkinnedDrawerHelper::skinAttributesInit(this, inputAttributes);
}

void SkinnedNormalMapForwardShader::difuseShaderChildsExtendedUniformsInit(){
	this->vertexShaderFilename = VSHADER;
	SkinnedDrawerHelper::skinUniformsInit(this, &this->jointsTransformUniform);
}