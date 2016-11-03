/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "RenderStage.h"
#include <list>
using namespace std;

class RenderPipeline{

private:
	vector<OutputResource*> lockedStagesResources; 
	list<RenderStage*> renderScheme;  // Contiene las stages del pipeline
	unsigned int recolectProfileDataTimeSlot;  // Time slot para saber si recolectar la informacion de profile
	void resolveWriteLocks(RenderStage* stage);
	void unlockAllOutputs();
	void clear();
	void identifyWriteLockStages();
	void generateLockStagesResourcesVector();
	void evaluateRenderStageDependences(OutputResource* output);
	void evaluateInputDependences(RenderStage* stage, list<OutputResource*>* inputs, OutputResource* output);
	void clearLocks();
	void addStageOutputs(RenderStage* stage);
	void updateStageOutputs(RenderStage* stage);
	// Se valida que existan todas las dependencias(inputs) del stage en el pipeline
	void validateRenderStageInPipeline(RenderStage* renderStage);
	// Comprueba si el input existe en algún stage del pipeline
	bool existsInputInPipeline(OutputResource* input);
	// Comprueba si el output existe en el render stage
	bool isOutputInRenderStage(OutputResource* output, RenderStage* stage);
	// Limpia los stages del pipeline para que queden como nuevos
	void cleanStages();
	// Toma los valores del profile y se los pasa al analizador de performance para que los guarde y procese
	void saveFrameProfileResults();

protected:
	// Agrega un stage al pipeline
	void addRenderStage(RenderStage* renderStage);
	// Agrega todas las stages de la lista al pipeline
	void addAllStagesToPipeline(list<RenderStage*>* stages);
	// Configura en clases hijas todas las stages del pipeline
	virtual void configurePipelineScheme() = 0;

public:
	RenderPipeline();
	virtual ~RenderPipeline();

	// Renderiza el pipeline
	void render();	

	// Debe llamarse si es que se cambio la configuracion del pipeline (se agregaron o sacaraon stages por ejemplo)
	void updatePipelineScheme();
};