/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "FrameBuffer.h"
#include "OutputResource.h"
#include "GpuTimeProfiler.h"
#include <list>
#include <map>
using namespace std;

class RenderStage{

private:
	vector<OutputResource*> outputs;

	// Cada stage lleva un handler para medir cuanto tiempo tarda en renderizarse
	unsigned int profileHandler;

protected:
	// La mayoria de los render stage (todos diria yo) precisan saber el tamaño de pantalla
	vec2 screenSize;

	// Agrega al vector de outputs del stage, un output mas
	void addOutputResource(OutputResource* output){ this->outputs.push_back(output); }
	// Actualiza la textura en el OutputResource indicado por index
	void updateOutputResource(MemoryTexture* texture, int index){ this->outputs.at(index)->setOutputTexture(texture); }
	// Actualiza la textura en el OutputResource indicado por index
	void allowsUnlockOnOutputResource(bool allowsUnlock, int index){ this->outputs.at(index)->setUnlockAllowed(allowsUnlock); }

public:
	RenderStage(){ this->profileHandler = GpuTimeProfiler::reservProfile(); }

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

	// Consigue los outputs del stage
	vector<OutputResource*>* getOutputs(){ return &this->outputs; }

	// Utilizado para hacer profiling de gpu del stage actual
	unsigned int getProfileHandler(){ return this->profileHandler; }

	// Borra los outputs de la memoria
	void deleteOutputs(){
		for(unsigned int i=0; i < this->outputs.size(); i++){
			delete this->outputs[i];
		}
		this->outputs.clear();
	}

	virtual ~RenderStage(){	this->deleteOutputs();	}
};