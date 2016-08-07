/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SkinnedDifuseShader.h"
#include "SkinnedDifuseDeferredShader.h"
#include "SkinnedDifuseForwardShader.h"

SkinnedDifuseShader* SkinnedDifuseShader::instance = NULL;

SkinnedDifuseShader* SkinnedDifuseShader::getInstance(){
	if(instance==NULL){
		instance = new SkinnedDifuseShader();
		instance->init();
	}
	return instance;
}

MeshDrawerShader* SkinnedDifuseShader::configureDeferredShadingImplementation(){ return new SkinnedDifuseDeferredShader(); }
MeshDrawerShader* SkinnedDifuseShader::configureForwardShadingImplementation(){ return new SkinnedDifuseForwardShader(); }
