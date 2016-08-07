/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "BackBufferWriterStage.h"
#include "ApiTexture.h"
#include "DeferredGeometryStage.h"
#include "SSAOShader.h"
#include "MergeTwoImagesShader.h"

class SSAORenderStage : public BackBufferWriterStage{

private:
	FrameBufferRequest* configureFBORequest(int width, int height);
	FrameBufferRequest* fboRequest;
	DeferredGeometryStage* geometryStage;
	unsigned int fetchsPerFragment;

	// FBOs de las diferentes pasadas de SSAO y etapa de merge
	FrameBuffer* frameBufferPass1;

	//Shaders de las diferentes pasadas de SSAO
	SSAOShader* ssaoShaderPass1;
	
public:
	// Tiene como dependencia un geometry stage de donde consume
	SSAORenderStage(DeferredGeometryStage* geometryStage, int screenWidth, int screenHeight);
	virtual ~SSAORenderStage();

	// Define el nombre del stage. Esto se utiliza para identificar al stage por ejemplo en el profiling
	string getName(){ return "SSAORenderStage"; }

	// Define sus outputs
	void configureStageOutputs();

	// Renderiza el render stage actual
	void render(bool isTheLastStage);

	// Agrega al vector de inputs los inputs de este Render Stage particular
	void addStageInputs(list<OutputResource*>* inputs);

	// Actualiza la informacion de las texturas de salida
	void updateOutputData();

	// Tiene como salida el back buffer con la informacion de ambient occlussion
	OutputResource* getBackBuffer(){ return this->getOutputs()->at(0); };	

	// Setea la dependencia de geometria
	void setGeometryStage(DeferredGeometryStage* stage){ this->geometryStage = stage; }

	// Setea la cantidad de fetchs por fragmento que se haran en el SSAO
	void setFetchsPerFragment(unsigned int count){ this->fetchsPerFragment = count; }
};

