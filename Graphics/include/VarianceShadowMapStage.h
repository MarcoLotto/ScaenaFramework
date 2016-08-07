/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "BackBufferWriterStage.h"
#include "Scene.h"
#include "SceneLight.h"

class VarianceShadowMapStage : public BackBufferWriterStage{

private:
	FrameBufferRequest* configureFBORequest(int width, int height);
	FrameBufferRequest* fboRequest;
	FrameBuffer* frameBuffer;
	ShadowManager* shadowManager;
	ObjectManager* objectManager;

	SceneLight* light;
	BackBufferWriterStage* filterStage;
	int prebankLoaded;
	
public:
	VarianceShadowMapStage(SceneLight* light, int mapWidth, int mapHeight);
	virtual ~VarianceShadowMapStage();

	// Define el nombre del stage. Esto se utiliza para identificar al stage por ejemplo en el profiling
	string getName(){ return "VarianceShadowMapStage"; }

	// Define sus outputs
	void configureStageOutputs();

	// Renderiza el render stage actual
	void render(bool isTheLastStage);

	// Agrega al vector de inputs los inputs de este Render Stage particular
	void addStageInputs(list<OutputResource*>* inputs);

	// Actualiza la informacion de las texturas de salida
	void updateOutputData();

	// Tiene como salida la textura del variance shadow map
	OutputResource* getBackBuffer(){ return this->getOutputs()->at(0); };	
		
	// Se setea el object manager a utilizar
	void setObjectManager(ObjectManager* objectManager){ this->objectManager = objectManager; }
	void setShadowManager(ShadowManager* shadowManager);

	// Permite indicar y obtener un stage filtro para el shadow map
	void setFilter(BackBufferWriterStage* filterStage){ this->filterStage = filterStage; }
	BackBufferWriterStage* getFilter(){ return this->filterStage; }

	// Indica si esta preparado el prebank de la sombra
	bool isPrebanckPrepared();

	// Indica si esta habilitado el prebank
	bool isPrebanckEnabled();
};