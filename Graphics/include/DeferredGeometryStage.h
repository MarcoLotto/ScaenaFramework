/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GeometryStage.h"

class DeferredGeometryStage : public GeometryStage{

private:
	FrameBufferRequest* configureFBORequest(int width, int height);
	FrameBufferRequest* fboRequest;
	FrameBuffer* frameBuffer;
	
public:
	DeferredGeometryStage(int screenWidth, int screenHeight);
	virtual ~DeferredGeometryStage();

	// Define el nombre del stage. Esto se utiliza para identificar al stage por ejemplo en el profiling
	string getName(){ return "DeferredGeometryStage"; }

	// Define sus outputs
	void configureStageOutputs();

	// Renderiza el render stage actual
	void render(bool isTheLastStage);

	// Agrega al vector de inputs los inputs de este Render Stage particular
	void addStageInputs(list<OutputResource*>* inputs);

	// Actualiza la informacion de las texturas de salida
	void updateOutputData();
	
	// Todas las salidas que tiene que tener un Deferred Geometry Stage
	OutputResource* getDepthBuffer(){ return this->getOutputs()->at(0); };
	OutputResource* getNormalBuffer(){ return this->getOutputs()->at(1); };
	OutputResource* getDifuseBuffer(){ return this->getOutputs()->at(2); };
	OutputResource* getMaterialBuffer(){ return this->getOutputs()->at(3); };
};
