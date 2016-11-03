/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshDrawerShader.h"

// Proxy para el shader real. Permite elegir que tipo de renderizado se desea (deferred, forward, etc)
class MeshDrawerShaderImplProxy : public MeshDrawerShader{

private:	
	MeshDrawerShader* currentShadingImplementation;	

	// Defino las implementaciones que se puede utilizar
	MeshDrawerShader* deferredShadingImplementation;
	MeshDrawerShader* forwardShadingImplementation;

protected:	
	void prepareForDraw(Mesh* mesh){};

	// Debo en clases hijas definir quienes son las instancias de forward y deferred
	virtual MeshDrawerShader* configureDeferredShadingImplementation() = 0;
	virtual MeshDrawerShader* configureForwardShadingImplementation() = 0;
	
public:	
	void init();

	//Dibuja el contenido de un mesh
	void draw(Mesh* mesh);

	//Dibuja el contenido de un mesh utilizando transform feedback
	void draw(Mesh* mesh, TransformFeedbackObject* transformFeedbackObject);

	// Permito seleccionar la implementacion a utilizar
	void useDeferredShadingImplementation(){ this->currentShadingImplementation = this->deferredShadingImplementation; }
	void useForwardShadingImplementation(){ this->currentShadingImplementation = this->forwardShadingImplementation; }

	MeshDrawerShader* getCurrentShadingImplementation(){ return this->currentShadingImplementation; }
};
