/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SkinnedParallaxDeferredShader.h"
#include "SkinnedDrawerHelper.h"
#include "SkinnedNormalMapShader.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/SkinnedParallaxDeferredShader.vert"
//********************************************************************************************************

SkinnedParallaxDeferredShader::SkinnedParallaxDeferredShader(){ 
	this->initOptimizationsParameters(); 
	this->setLowerShader(SkinnedNormalMapShader::getInstance()); //A que shader pasa al optimizar
}

void SkinnedParallaxDeferredShader::parallaxShaderChildsExtendedDraw(Mesh* mesh){
	SkinnedDrawerHelper::skinDraw(this, mesh, this->jointsTransformUniform);
}

void SkinnedParallaxDeferredShader::difuseShaderChildsExtendedAttributesInit(list<string>* inputAttributes){
	this->vertexShaderFilename = VSHADER;
	SkinnedDrawerHelper::skinAttributesInit(this, inputAttributes);
}

void SkinnedParallaxDeferredShader::difuseShaderChildsExtendedUniformsInit(){
	this->vertexShaderFilename = VSHADER;
	SkinnedDrawerHelper::skinUniformsInit(this, &this->jointsTransformUniform);
}