/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "RenderPipeline.h"
#include "RenderStageNotAssignedInPipeline.h"
#include "InputDoesNotExistInRenderPipeline.h"
#include "GraphicDevice.h"
#include "GpuTimeProfiler.h"
#include "SystemPerformanceAnalyzer.h"
#include "TimeManager.h"
#include "WindowConnector.h"

// Define cada cuanto tiempo de recojen los datos de profiling del frame
#define PROFILE_TIME_INTERVAL 4000

RenderPipeline::RenderPipeline(){
	this->recolectProfileDataTimeSlot = TimeManager::getInstance()->reservTimeSlot();
}
RenderPipeline::~RenderPipeline(){
	TimeManager::getInstance()->removeTimeSlot(this->recolectProfileDataTimeSlot);
}

// Se valida que existan todas las dependencias(inputs) del stage en el pipeline
void RenderPipeline::validateRenderStageInPipeline(RenderStage* renderStage){
	list<OutputResource*> inputs;
	renderStage->addStageInputs(&inputs);
	std::list<OutputResource*>::iterator it = inputs.begin();
	while(it != inputs.end()){
		if(!this->existsInputInPipeline(*it)){
			throw new InputDoesNotExistInRenderPipeline();
		}
		++it;
	}
}

// Comprueba si el input existe en algún stage del pipeline
bool RenderPipeline::existsInputInPipeline(OutputResource* input){
	for(std::list<RenderStage*>::iterator it = this->renderScheme.begin(); it != this->renderScheme.end(); it++){
		if(this->isOutputInRenderStage(input, *it)){
			return true;
		}
	}
	return false;
}

// Comprueba si el output existe en el render stage
bool RenderPipeline::isOutputInRenderStage(OutputResource* output, RenderStage* stage){
	for(int i=0; i < stage->getOutputs()->size(); i++){
		if(stage->getOutputs()->at(i) == output)
			return true;
	}
	return false;
}

void RenderPipeline::addRenderStage(RenderStage* renderStage){
	if(renderStage == NULL){
		throw new RenderStageNotAssignedInPipeline();
	}
	this->validateRenderStageInPipeline(renderStage);
	this->renderScheme.push_back(renderStage);
}

void RenderPipeline::render(){
	std::list<RenderStage*>::iterator it = this->renderScheme.begin();
	while(it != this->renderScheme.end()){
		RenderStage* stage = (*it);
		++it;
		// Inicio el profiling del stage
		GpuTimeProfiler::startProfile(stage->getProfileHandler());

		// Mando a renderizar el stage, informo si es el ultimo stage en el pipeline
		bool isTheLastStage = (it == this->renderScheme.end());
		stage->render(isTheLastStage);
		// Actualizo los outputs (las texturas de los mismos)
		this->updateStageOutputs(stage);
		// Resuelvo los writelocks
		this->resolveWriteLocks(stage);

		// Termino de rendirizar el stage, termino el profile
		GpuTimeProfiler::endProfile();
	}
	// Termine de recorrer el pipeline, aquellos con write lock lo dejaran de tener
	this->unlockAllOutputs();

	// Termine de renderizar el frame
	WindowConnector::SwapBuffers();

	// Logueo los resultados del profile del frame
	this->saveFrameProfileResults();
}

void RenderPipeline::saveFrameProfileResults(){
	if(TimeManager::getInstance()->getElapsedTime(this->recolectProfileDataTimeSlot) >= PROFILE_TIME_INTERVAL){
		TimeManager::getInstance()->updateTimeSlot(this->recolectProfileDataTimeSlot);

		SystemPerformanceAnalyzer* performanceAnalyzer = SystemPerformanceAnalyzer::getInstance();
		performanceAnalyzer->indicateProfilingStart("Frame profiling");
		std::list<RenderStage*>::iterator it = this->renderScheme.begin();
		while(it != this->renderScheme.end()){
			RenderStage* stage = (*it);
			++it;
			float timeInMilliseconds = (float) GpuTimeProfiler::requestProfileResult(stage->getProfileHandler()) / 1000000.0f;
			performanceAnalyzer->updateProfileData(stage->getName(), timeInMilliseconds);
		}
		performanceAnalyzer->indicateProfilingEnd();
	}
}

void RenderPipeline::resolveWriteLocks(RenderStage* stage){
	std::vector<OutputResource*>::iterator it = this->lockedStagesResources.begin();
	while(it != this->lockedStagesResources.end()){
		OutputResource* output = (*it);
		// Si estoy en la stage que le corresponde para liberar y esta permitido, lo libero
		if(output->getFreeWriteLockStage() == stage && output->isUnlockAllowed()){
			output->getOutputTexture()->setWriteLock(false);
		}
		++it;
	}
}

void RenderPipeline::clear(){
	this->renderScheme.clear();
	this->clearLocks();
}

void RenderPipeline::clearLocks(){
	std::vector<OutputResource*>::iterator it = this->lockedStagesResources.begin();
	while(it != this->lockedStagesResources.end()){
		MemoryTexture* texture = (*it)->getOutputTexture();
		if(texture != NULL){
			texture->setWriteLock(false);
		}
		it = this->lockedStagesResources.erase(it);
	}
}

void RenderPipeline::unlockAllOutputs(){
	std::vector<OutputResource*>::iterator it = this->lockedStagesResources.begin();
	while(it != this->lockedStagesResources.end()){
		OutputResource* output = (*it);
		// Si esta permitido liberar este recurso, lo libero (sin importar la stage)
		if(output->isUnlockAllowed()){
			output->getOutputTexture()->setWriteLock(false);
		}
		++it;
	}
}

void RenderPipeline::updatePipelineScheme(){
	//Primero borro el pipeline actual
	this->clear();

	//Actualizo al nuevo pipeline
	this->configurePipelineScheme();
	
	// Limpio los stages por si estan usados
	this->cleanStages();

	// Identifico en donde se libera cada writelock
	this->identifyWriteLockStages();
}

void RenderPipeline::cleanStages(){
	std::list<RenderStage*>::iterator it = this->renderScheme.begin();
	while(it != this->renderScheme.end()){
		RenderStage* stage = *it;
		stage->deleteOutputs();
		stage->configureStageOutputs();
		++it;
	}
}

void RenderPipeline::identifyWriteLockStages(){
	// Primero genero el vector de locks con los outputs de cada stage
	this->generateLockStagesResourcesVector();

	// Por cada output, recorro el pipeline buscando dependencias al mismo
	std::vector<OutputResource*>::iterator it = this->lockedStagesResources.begin();
	while(it != this->lockedStagesResources.end()){
		this->evaluateRenderStageDependences(*it);
		++it;
	}	
}

void RenderPipeline::generateLockStagesResourcesVector(){
	this->clearLocks();
	// Recorro cada stage, le pido sus outputs y los guardo en el vector de lock
	std::list<RenderStage*>::iterator it = this->renderScheme.begin();
	while(it != this->renderScheme.end()){
		RenderStage* stage = (*it);
		this->addStageOutputs(stage);
		++it;
	}
}

void RenderPipeline::evaluateRenderStageDependences(OutputResource* output){
	list<OutputResource*>* inputs = new list<OutputResource*>();
	// Para cada render stage, verifico si tiene como dependencia al output
	std::list<RenderStage*>::iterator it = this->renderScheme.begin();
	while(it != this->renderScheme.end()){
		RenderStage* stage = (*it);
		// Pido sus dependencias y las mando a evaluar
		stage->addStageInputs(inputs);
		this->evaluateInputDependences(stage, inputs, output);
		inputs->clear();
		++it;
	}
	delete inputs;
}

void RenderPipeline::evaluateInputDependences(RenderStage* stage, list<OutputResource*>* inputs, OutputResource* output){
	// Busco si alguna de las dependencias es el output. Si es asi actualizo el output.
	std::list<OutputResource*>::iterator it = inputs->begin();
	while(it != inputs->end()){
		OutputResource* input = (*it);
		if(input == output){
			output->setFreeWriteLockStage(stage);
		}
		++it;
	}
}

void RenderPipeline::addStageOutputs(RenderStage* stage){
	// Agrego cada output del stage a la lista de locked outputs
	std::vector<OutputResource*>::iterator it = stage->getOutputs()->begin();
	while(it != stage->getOutputs()->end()){
		// Asigno la dependencia a si mismo por si no se usa
		(*it)->setFreeWriteLockStage(stage);
		this->lockedStagesResources.push_back(*it);
		++it;
	}
}

void RenderPipeline::updateStageOutputs(RenderStage* stage){
	// Actualizo la informacion del output del stage
	stage->updateOutputData();

	// Por cada output del stage actualizo su MemoryTexture
	std::vector<OutputResource*>::iterator it = stage->getOutputs()->begin();
	while(it != stage->getOutputs()->end()){		
		// Primero trato de encontrar cacheado el output
		OutputResource* output = (*it);
		OutputResource* probableOutput = this->lockedStagesResources.at(output->getIndexChache());
		if(output == probableOutput){
			probableOutput->setOutputTexture(output->getOutputTexture());
		}
		else{
			// No encontre el output cacheado, lo busco y lo cacheo
			for(int i=0; i < this->lockedStagesResources.size(); i++){
				OutputResource* foundOutput = this->lockedStagesResources.at(i);
				if(foundOutput == output){
					output->setIndexCache(i);
					foundOutput->setOutputTexture(output->getOutputTexture());
					break;
				}
			}
		}
		++it;
	}
}

// Agrega todas las stages de la lista al pipeline
void RenderPipeline::addAllStagesToPipeline(list<RenderStage*>* stages){
	std::list<RenderStage*>::iterator it = stages->begin();
	while(it != stages->end()){
		this->addRenderStage(*it);
		++it;
	}
}