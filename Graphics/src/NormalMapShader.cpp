/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "NormalMapShader.h"
#include "GraphicDevice.h"
#include "NormalMapForwardShader.h"
#include "NormalMapDeferredShader.h"

NormalMapShader* NormalMapShader::instance = NULL;

NormalMapShader* NormalMapShader::getInstance(){
	if(instance==NULL){
		instance = new NormalMapShader();
		instance->init();		
	}
	return instance;
}

MeshDrawerShader* NormalMapShader::configureDeferredShadingImplementation(){ return new NormalMapDeferredShader(); }
MeshDrawerShader* NormalMapShader::configureForwardShadingImplementation(){ return new NormalMapForwardShader(); }