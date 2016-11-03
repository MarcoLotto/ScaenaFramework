/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ParallaxShader.h"
#include "GraphicDevice.h"
#include "ParallaxDeferredShader.h"
#include "ParallaxForwardShader.h"

ParallaxShader* ParallaxShader::instance = NULL;

ParallaxShader* ParallaxShader::getInstance(){
	if(instance==NULL){
		instance = new ParallaxShader();
		instance->init();
	}
	return instance;
}

MeshDrawerShader* ParallaxShader::configureDeferredShadingImplementation(){ return new ParallaxDeferredShader(); }
MeshDrawerShader* ParallaxShader::configureForwardShadingImplementation(){ return new ParallaxForwardShader(); }


