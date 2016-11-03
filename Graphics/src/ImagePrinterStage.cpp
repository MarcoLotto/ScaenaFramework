/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ImagePrinterStage.h"
#include "GraphicDevice.h"
#include "FrameBufferManager.h"
#include "UIShader.h"

ImagePrinterStage::ImagePrinterStage(BackBufferWriterStage* previousStage, int screenWidth, int screenHeight){
	this->frameBuffer = NULL;	
	this->fboRequest = this->configureFBORequest(screenWidth, screenHeight);
	this->configureStageOutputs();
	this->previousStage = previousStage;	
	this->screenSize = vec2(screenWidth, screenHeight);
}

ImagePrinterStage::~ImagePrinterStage(){
	delete this->fboRequest;
}
	
// Actaliza la informacion de las texturas de sus outputs
void ImagePrinterStage::configureStageOutputs(){
	this->addOutputResource(new OutputResource(NULL));  // La imagen copiada
}

// Renderiza el render stage actual
void ImagePrinterStage::render(bool isTheLastStage){
	// Cambio el viewport al corecto segun el tamaño de imagen
	GraphicDevice::getInstance()->setViewport(0,0, this->screenSize.x, this->screenSize.y);

	// Desactivo el control de depth y activo el blending
	GraphicDeviceConstantCatalog* apiConstants = GraphicDevice::getInstance()->getConstantCatalog();
	GraphicDevice::getInstance()->disableMode(apiConstants->getDepthTestFlag());
	GraphicDevice::getInstance()->enableMode(apiConstants->getBlendFlag());
	GraphicDevice::getInstance()->blendFunction(apiConstants->getBlendSrcAlphaFlag(), apiConstants->getBlendOneMinusSrcAlphaFlag());
	
	// Mezclo el contenido del stage previo, con la interfaz de usuario
	this->frameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequest, this->frameBuffer, isTheLastStage);
	GraphicDevice::getInstance()->clearColorBuffer();

	// Renderizo la imagen	
	UIShader::getInstance()->drawSideCenteredCuad(this->previousStage->getBackBuffer()->getOutputTexture(), vec2(1.0f), vec2(0.0f), mat4(1.0f), 1.0f);	
	
	GraphicDevice::getInstance()->disableMode(apiConstants->getBlendFlag());
	GraphicDevice::getInstance()->enableMode(apiConstants->getDepthTestFlag());
}

// Agrega al vector de inputs los inputs de este Render Stage particular
void ImagePrinterStage::addStageInputs(list<OutputResource*>* inputs){
	inputs->push_back(this->previousStage->getBackBuffer());
}

void ImagePrinterStage::updateOutputData(){
	this->updateOutputResource(this->frameBuffer->getOutputTextures()->at(0), 0); // La imagen copiada
}

FrameBufferRequest* ImagePrinterStage::configureFBORequest(int width, int height){
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
