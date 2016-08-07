/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UserInterfaceStage.h"
#include "GraphicDevice.h"
#include "FrameBufferManager.h"

UserInterfaceStage::UserInterfaceStage(BackBufferWriterStage* previousStage, int screenWidth, int screenHeight){
	this->mergeFrameBuffer = NULL;
	this->fboRequest = this->configureFBORequest(screenWidth, screenHeight);
	this->configureStageOutputs();
	this->previousStage = previousStage;
	this->uiController = NULL;
	this->screenSize = vec2(screenWidth, screenHeight);
    this->cleanScreen = true;
}

UserInterfaceStage::UserInterfaceStage(int screenWidth, int screenHeight, bool cleanScreen){
    this->mergeFrameBuffer = NULL;
    this->fboRequest = this->configureFBORequest(screenWidth, screenHeight);
    this->configureStageOutputs();
    this->previousStage = NULL;
    this->uiController = NULL;
    this->screenSize = vec2(screenWidth, screenHeight);
    this->cleanScreen = cleanScreen;
}

UserInterfaceStage::~UserInterfaceStage(){
	delete this->fboRequest;

	// Borro el frame buffer creado a mano y su textura asociada
	delete this->uiDataFrameBuffer->getOutputTextures()->at(0);
	delete this->uiDataFrameBuffer;
}
	
// Actaliza la informacion de las texturas de sus outputs
void UserInterfaceStage::configureStageOutputs(){
	this->addOutputResource(new OutputResource(NULL));  // La escena con UI
}

// Renderiza el render stage actual
void UserInterfaceStage::render(bool isTheLastStage){
	// Cambio el viewport al corecto segun el tamaño de imagen
	GraphicDevice::getInstance()->setViewport(0,0, this->screenSize.x, this->screenSize.y);

	// Desactivo el control de depth y activo el blending
	GraphicDeviceConstantCatalog* apiConstants = GraphicDevice::getInstance()->getConstantCatalog();
	GraphicDevice::getInstance()->disableMode(apiConstants->getDepthTestFlag());
	GraphicDevice::getInstance()->enableMode(apiConstants->getBlendFlag());
	GraphicDevice::getInstance()->blendFunction(apiConstants->getBlendSrcAlphaFlag(), apiConstants->getBlendOneMinusSrcAlphaFlag());
	
	// Mezclo el contenido del stage previo, con la interfaz de usuario
	this->mergeFrameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequest, this->mergeFrameBuffer, isTheLastStage || this->previousStage==NULL);
    if(this->cleanScreen){
        GraphicDevice::getInstance()->clearColorBuffer();
    }
	// Primero renderizo el fondo
    if(this->previousStage != NULL){
        UIShader::getInstance()->drawSideCenteredCuad(this->previousStage->getBackBuffer()->getOutputTexture(), vec2(1.0f), vec2(0.0f), mat4(1.0f), 1.0f);
    }
	this->uiController->draw();

	GraphicDevice::getInstance()->disableMode(apiConstants->getBlendFlag());
	GraphicDevice::getInstance()->enableMode(apiConstants->getDepthTestFlag());
}

// Agrega al vector de inputs los inputs de este Render Stage particular
void UserInterfaceStage::addStageInputs(list<OutputResource*>* inputs){
    if(this->previousStage != NULL)
        inputs->push_back(this->previousStage->getBackBuffer());
}

void UserInterfaceStage::updateOutputData(){
	this->updateOutputResource(this->mergeFrameBuffer->getOutputTextures()->at(0), 0); // La escena con UI
}

FrameBufferRequest* UserInterfaceStage::configureFBORequest(int width, int height){
	GraphicDevice* device = GraphicDevice::getInstance();
	GraphicDeviceConstantCatalog* catalog = device->getConstantCatalog();

	//Genero el FrameBufferRequest
	FrameBufferRequest* request = new FrameBufferRequest();
	
	//Mantener con GL_LINEAR para que los post efectos hagan la mitad de fetchs
	//Mantener GL_CLAMP_TO_EDGE para que no se vea el otro borde al aplicar blur
	TextureRequest* positionRequest = new TextureRequest(vec2(width, height), catalog->getFormatRGB(), catalog->getFormatRGB32F());
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMinFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMagFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapSFlag(), catalog->getTextureClampToEdgeFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapTFlag(), catalog->getTextureClampToEdgeFlag()));
	request->addTextureRequest(positionRequest);
	return request;
}
