/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "DifuseShader.h"
#include "DifuseDeferredShader.h"
#include "DifuseForwardShader.h"

DifuseShader* DifuseShader::instance = NULL;

DifuseShader* DifuseShader::getInstance(){
	if(instance==NULL){
		instance = new DifuseShader();
		instance->init();
	}
	return instance;
}

MeshDrawerShader* DifuseShader::configureDeferredShadingImplementation(){ return new DifuseDeferredShader(); }
MeshDrawerShader* DifuseShader::configureForwardShadingImplementation(){ return new DifuseForwardShader(); }

