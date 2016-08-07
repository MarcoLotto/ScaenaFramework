/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "BackBufferWriterStage.h"
#include "GeometryStage.h"
#include "ApiTexture.h"
#include "Scene.h"

// Dada una escena, la renderiza e ilumina, utilizando la tecnica de forwarding shading
class ForwardGeometryStage : public BackBufferWriterStage, public GeometryStage {

private:
	FrameBufferRequest* configureFBORequest(int width, int height, bool manageDepthAsTexture, int outTextureExternalFormat, int outTextureInternalFormat);
	void initClass(int screenWidth, int screenHeight, bool manageDepthAsTexture, int outTextureExternalFormat, int outTextureInternalFormat);

	FrameBufferRequest* fboRequest;
	FrameBuffer* frameBuffer;
    bool manageDepthAsTexture;
	bool blendingEnabled;
    bool forceRenderToScreen;
		
public:
	ForwardGeometryStage(int screenWidth, int screenHeight, bool manageDepthAsTexture);
	ForwardGeometryStage(int screenWidth, int screenHeight, bool manageDepthAsTexture, int outTextureExternalFormat, int outTextureInternalFormat);
	virtual ~ForwardGeometryStage();

	// Define el nombre del stage. Esto se utiliza para identificar al stage por ejemplo en el profiling
	string getName(){ return "ForwardGeometryStage"; }

	// Define sus outputs
	void configureStageOutputs();

	// Renderiza el render stage actual
	void render(bool isTheLastStage);

	// Agrega al vector de inputs los inputs de este Render Stage particular
	void addStageInputs(list<OutputResource*>* inputs);

	// Actualiza la informacion de las texturas de salida
	void updateOutputData();

	// Determina si estara habilitada la transparencia en los objetos
	void setBlendingEnabled(bool value){ this->blendingEnabled = value; }
    
    // Determina si forzar a que el stage renderice a la pantalla sin importar que haya mas stages en el pipeline (desabilitado por defecto)
    void setForceRenderToScreen(bool value){ this->forceRenderToScreen = value; }

	// Como es un GeometryStage, debe tener como salida al depth buffer
    OutputResource* getDepthBuffer();

	// Como es tambien un back buffer writer, tiene como salida la escena renderizada e iluminada
    OutputResource* getBackBuffer();
};