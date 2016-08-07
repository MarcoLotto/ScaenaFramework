/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "BackBufferWriterStage.h"
#include "ApiTexture.h"
#include "DeferredGeometryStage.h"
#include "Scene.h"
#include "SSAORenderStage.h"

class PhongLightingStage : public BackBufferWriterStage{

private:
	FrameBufferRequest* configureFBORequest(int width, int height);
	FrameBufferRequest* fboRequest;
	FrameBuffer* frameBuffer;
	DeferredGeometryStage* geometryStage;
	BackBufferWriterStage* ssaoStage;  // REVIEW: El tipo esta mal!!
	Scene* scene;
		
public:
	// Tiene como dependencia un geometry stage de donde consume  REVIEW: El tipo del ssaoStage esta mal!
	PhongLightingStage(DeferredGeometryStage* geometryStage, int screenWidth, int screenHeight, BackBufferWriterStage* ssaoStage=NULL);
	virtual ~PhongLightingStage();

	// Define el nombre del stage. Esto se utiliza para identificar al stage por ejemplo en el profiling
	string getName(){ return "PhongLightingStage"; }

	// Define sus outputs
	void configureStageOutputs();

	// Renderiza el render stage actual
	void render(bool isTheLastStage);

	// Agrega al vector de inputs los inputs de este Render Stage particular
	void addStageInputs(list<OutputResource*>* inputs);

	// Actualiza la informacion de las texturas de salida
	void updateOutputData();

	// Tiene como salida el back buffer con la escena final iluminada
	OutputResource* getBackBuffer(){ return this->getOutputs()->at(0); };	

	// Setea la dependencia de geometria
	void setGeometryStage(DeferredGeometryStage* stage){ this->geometryStage = stage; }

	// Se setea la escena a iluminar
	void setScene(Scene* scene){ this->scene = scene; }
};