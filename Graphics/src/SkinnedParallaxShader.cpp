/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SkinnedParallaxShader.h"
#include "GraphicDevice.h"
#include "SkinnedParallaxDeferredShader.h"
#include "SkinnedParallaxForwardShader.h"

SkinnedParallaxShader* SkinnedParallaxShader::instance = NULL;

SkinnedParallaxShader* SkinnedParallaxShader::getInstance(){
	if(instance==NULL){
		instance = new SkinnedParallaxShader();
		instance->init();
	}
	return instance;
}

MeshDrawerShader* SkinnedParallaxShader::configureDeferredShadingImplementation(){ return new SkinnedParallaxDeferredShader(); }
MeshDrawerShader* SkinnedParallaxShader::configureForwardShadingImplementation(){ return new SkinnedParallaxForwardShader(); }
