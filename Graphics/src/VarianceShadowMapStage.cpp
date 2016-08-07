/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "VarianceShadowMapStage.h"
#include "GraphicDevice.h"
#include "FrameBufferManager.h"

#define PI 3.1415927f
#define PREBANCK_RENDER_TIMES 2

VarianceShadowMapStage::VarianceShadowMapStage(SceneLight* light, int mapWidth, int mapHeight){
	this->frameBuffer = NULL;
	this->fboRequest = this->configureFBORequest(mapWidth, mapHeight);
	this->configureStageOutputs();
	this->objectManager = NULL;
	this->shadowManager = NULL;
	this->screenSize = vec2(mapWidth, mapHeight);
	this->light = light;
	this->filterStage = NULL;
	this->prebankLoaded = 0;
}
VarianceShadowMapStage::~VarianceShadowMapStage(){
	delete this->fboRequest;
}

void VarianceShadowMapStage::setShadowManager(ShadowManager* shadowManager){ 
	this->shadowManager = shadowManager;
}
// Renderiza el render stage actual
void VarianceShadowMapStage::render(bool isTheLastStage){
	// REVIEW: Al utilizar prebanck precisa dos pasadas para guardar el prebank, pero no se porque
	if(!this->isPrebanckEnabled() || !this->isPrebanckPrepared()){
		GraphicDevice* device = GraphicDevice::getInstance();
		GraphicDeviceConstantCatalog* catalog = device->getConstantCatalog();
	
		// Cambio al viewport para el tamaño de la textura
		device->setViewport(0,0, this->screenSize.x, this->screenSize.y);	

		// Como existe el prebankeado, me aseguro que la textura este liberada
		if(this->frameBuffer != NULL){
			this->frameBuffer->getOutputTextures()->at(0)->setWriteLock(false);
		}

		// Pido y bindeo un frame buffer para generar el shadow map
		this->frameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequest, this->frameBuffer);
		
		GraphicDevice::getInstance()->clearColorAndDepthBuffer();	

		this->shadowManager->updateLightShadowView(this->light);
		ShadowShader* shadowShader = ShadowShader::getInstance();
		this->objectManager->renderWithSpecificShader(shadowShader, this->light->getShadowData()->getShadowCamera());
		
		// Ya renderice, por lo que tengo la textura guardada para hacer prebanck
		if(this->prebankLoaded < PREBANCK_RENDER_TIMES){
			this->prebankLoaded++;  
		}
	}
	// Me aseguro que la textura tenga writelock
	this->frameBuffer->getOutputTextures()->at(0)->setWriteLock(true);
}

// Agrega al vector de inputs los inputs de este Render Stage particular
void VarianceShadowMapStage::addStageInputs(list<OutputResource*>* inputs){
	// No tiene inputs
}

// Actualiza la informacion de las texturas de salida
void VarianceShadowMapStage::updateOutputData(){
	this->updateOutputResource(this->frameBuffer->getOutputTextures()->at(0), 0); // El map con el variance shadow
	this->updateOutputResource(this->frameBuffer->getDepthBuffer(), 1); // El depth (debe liberarse)

	//Si esta activado el prebankeado, no tengo que actualizar la sombra, por lo que tengo que evitar que me la pisen
	this->allowsUnlockOnOutputResource(!this->light->getShadowData()->isPrebanked(), 0);
}

// Actaliza la informacion de las texturas de sus outputs
void VarianceShadowMapStage::configureStageOutputs(){
	this->addOutputResource(new OutputResource(NULL));  // El map con el variance shadow
	this->addOutputResource(new OutputResource(NULL));  // El depth (debe liberarse)
	
	// Esto se llama cada vez que se hace update al pipeline, por lo que tengo que volver a prebankear
	this->prebankLoaded = false;
}

FrameBufferRequest* VarianceShadowMapStage::configureFBORequest(int width, int height){
	GraphicDevice* device = GraphicDevice::getInstance();
	GraphicDeviceConstantCatalog* catalog = device->getConstantCatalog();

	//Genero el FrameBufferRequest
	FrameBufferRequest* request = new FrameBufferRequest();
	
	//Solicito un depth buffer
	request->setDepthBufferRequest(new TextureRequest(vec2(width, height), NULL, NULL));

	TextureRequest* positionRequest = new TextureRequest(vec2(width, height), catalog->getFormatRG(), catalog->getFormatRG32F());
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMinFilterFlag(), catalog->getTextureNearestFilterFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMagFilterFlag(), catalog->getTextureNearestFilterFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapSFlag(), catalog->getTextureClampToEdgeFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapTFlag(), catalog->getTextureClampToEdgeFlag()));
	request->addTextureRequest(positionRequest);
	return request;
}

// Indica si esta preparado el prebank de la sombra
bool VarianceShadowMapStage::isPrebanckPrepared(){
	return (this->prebankLoaded >= PREBANCK_RENDER_TIMES);
}

// Indica si esta habilitado el prebank
bool VarianceShadowMapStage::isPrebanckEnabled(){
	return this->light->getShadowData()->isPrebanked();
}