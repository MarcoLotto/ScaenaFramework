/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SkinnedNormalMapShader.h"
#include "GraphicDevice.h"
#include "SkinnedNormalMapForwardShader.h"
#include "SkinnedNormalMapDeferredShader.h"

SkinnedNormalMapShader* SkinnedNormalMapShader::instance = NULL;

SkinnedNormalMapShader* SkinnedNormalMapShader::getInstance(){
	if(instance==NULL){
		instance = new SkinnedNormalMapShader();
		instance->init();		
	}
	return instance;
}

MeshDrawerShader* SkinnedNormalMapShader::configureDeferredShadingImplementation(){ return new SkinnedNormalMapDeferredShader(); }
MeshDrawerShader* SkinnedNormalMapShader::configureForwardShadingImplementation(){ return new SkinnedNormalMapForwardShader(); }