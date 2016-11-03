/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "RenderStage.h"

class BackBufferWriterStage : virtual public RenderStage{

public:
	BackBufferWriterStage() : RenderStage(){}

	// Define el nombre del stage. Esto se utiliza para identificar al stage por ejemplo en el profiling
	virtual string getName() = 0;

	// Define sus outputs
	virtual void configureStageOutputs() = 0;

	// Renderiza el render stage actual
	virtual void render(bool isTheLastStage) = 0;

	// Agrega al vector de inputs los inputs de este Render Stage particular
	virtual void addStageInputs(list<OutputResource*>* inputs) = 0;

	// Actualiza la informacion de las texturas de salida
	virtual void updateOutputData() = 0;

	// Tiene como salida el back buffer
	virtual OutputResource* getBackBuffer() = 0;	
};