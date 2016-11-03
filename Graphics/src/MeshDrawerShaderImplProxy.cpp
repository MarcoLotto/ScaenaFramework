/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "GraphicDevice.h"
#include "MeshDrawerShaderImplProxy.h"

void MeshDrawerShaderImplProxy::draw(Mesh* mesh){ 
	this->currentShadingImplementation->setDistanceFactor(this->getDistanceFactor());
	this->currentShadingImplementation->draw(mesh); 
}

void MeshDrawerShaderImplProxy::draw(Mesh* mesh, TransformFeedbackObject* transformFeedbackObject){
	this->currentShadingImplementation->setDistanceFactor(this->getDistanceFactor());
	this->currentShadingImplementation->draw(mesh, transformFeedbackObject); 
}

void MeshDrawerShaderImplProxy::init(){
	this->currentShadingImplementation = NULL;

	// Creo cada una de las tecnicas de renderizado disponibles para este shader y las inicializo
	this->deferredShadingImplementation = this->configureDeferredShadingImplementation();
	this->deferredShadingImplementation->init();
	this->forwardShadingImplementation = this->configureForwardShadingImplementation();
	this->forwardShadingImplementation->init();

	// Tomo los parametros de optimizacion de alguna de las implementaciones (es indiferente de cual)
	this->setDistanceToChangeShader(this->deferredShadingImplementation->getDistanceToChangeShader());
	this->setLowerShader(this->deferredShadingImplementation->getLowerShader());
}