/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SkinnedParallaxForwardShader.h"
#include "SkinnedDrawerHelper.h"
#include "SkinnedNormalMapShader.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/SkinnedParallaxForwardShader.vert"
//********************************************************************************************************

SkinnedParallaxForwardShader::SkinnedParallaxForwardShader(){ 
	this->initOptimizationsParameters(); 
	this->setLowerShader(SkinnedNormalMapShader::getInstance()); //A que shader pasa al optimizar
}

void SkinnedParallaxForwardShader::parallaxShaderChildsExtendedDraw(Mesh* mesh){
	SkinnedDrawerHelper::skinDraw(this, mesh, this->jointsTransformUniform);
}

void SkinnedParallaxForwardShader::difuseShaderChildsExtendedAttributesInit(list<string>* inputAttributes){
	this->vertexShaderFilename = VSHADER;
	SkinnedDrawerHelper::skinAttributesInit(this, inputAttributes);
}

void SkinnedParallaxForwardShader::difuseShaderChildsExtendedUniformsInit(){
	this->vertexShaderFilename = VSHADER;
	SkinnedDrawerHelper::skinUniformsInit(this, &this->jointsTransformUniform);
}