/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SkinnedNormalMapDeferredShader.h"
#include "SkinnedDrawerHelper.h"
#include "SkinnedDifuseShader.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/SkinnedNormalMapDeferredShader.vert"
//********************************************************************************************************

SkinnedNormalMapDeferredShader::SkinnedNormalMapDeferredShader(){ 
	this->initOptimizationsParameters(); 
	this->setLowerShader(SkinnedDifuseShader::getInstance()); //A que shader pasa al optimizar
}

void SkinnedNormalMapDeferredShader::normalMapShaderChildsExtendedDraw(Mesh* mesh){
	SkinnedDrawerHelper::skinDraw(this, mesh, this->jointsTransformUniform);
}

void SkinnedNormalMapDeferredShader::difuseShaderChildsExtendedAttributesInit(list<string>* inputAttributes){
	this->vertexShaderFilename = VSHADER;
	SkinnedDrawerHelper::skinAttributesInit(this, inputAttributes);
}

void SkinnedNormalMapDeferredShader::difuseShaderChildsExtendedUniformsInit(){
	this->vertexShaderFilename = VSHADER;
	SkinnedDrawerHelper::skinUniformsInit(this, &this->jointsTransformUniform);
}