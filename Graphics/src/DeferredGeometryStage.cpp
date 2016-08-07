/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "DeferredGeometryStage.h"
#include "GraphicDevice.h"
#include "FrameBufferManager.h"
#include "ShaderManager.h"

DeferredGeometryStage::DeferredGeometryStage(int screenWidth, int screenHeight){
	this->fboRequest = this->configureFBORequest(screenWidth, screenHeight);
	this->configureStageOutputs();
	this->scene = NULL;
	this->frameBuffer = NULL;
	this->screenSize = vec2(screenWidth, screenHeight);
}
DeferredGeometryStage::~DeferredGeometryStage(){
	delete fboRequest;
}

// Define los outputs
void DeferredGeometryStage::configureStageOutputs(){
	this->addOutputResource(new OutputResource(NULL));  // Depth
	this->addOutputResource(new OutputResource(NULL));  // Normal
	this->addOutputResource(new OutputResource(NULL));  // Difuso
	this->addOutputResource(new OutputResource(NULL));  // Material
}

void DeferredGeometryStage::render(bool isTheLastStage){
	// Cambio el viewport al corecto segun el tama�o de imagen
	GraphicDevice::getInstance()->setViewport(0,0, this->screenSize.x, this->screenSize.y);

	// Indico a los shaders que se debe utilizar la estrategia de renderizado de deferred shading
	ShaderManager::getInstance()->useDeferredShadingStrategy();

	this->frameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequest, this->frameBuffer);
	GraphicDevice::getInstance()->clearColorAndDepthBuffer();	

	this->scene->update();
	this->scene->renderFromCurrentCamera();
}

// Agrega al vector de inputs los inputs de este Render Stage particular
void DeferredGeometryStage::addStageInputs(list<OutputResource*>* inputs){
	// No tiene ningun input
}

// Actualiza la informacion de las texturas de salida para matchear el frame buffer
void DeferredGeometryStage::updateOutputData(){
	this->updateOutputResource(this->frameBuffer->getDepthBuffer(), 0);			   // Depth
	this->updateOutputResource(this->frameBuffer->getOutputTextures()->at(0), 1);  // Normal
	this->updateOutputResource(this->frameBuffer->getOutputTextures()->at(1), 2);  // Difuso
	this->updateOutputResource(this->frameBuffer->getOutputTextures()->at(2), 3);  // Material
}

FrameBufferRequest* DeferredGeometryStage::configureFBORequest(int width, int height){
	GraphicDevice* device = GraphicDevice::getInstance();
	GraphicDeviceConstantCatalog* catalog = device->getConstantCatalog();
	vec2 screenSize = vec2(width, height);

	//Genero el FrameBufferRequest
	FrameBufferRequest* fboRequest = new FrameBufferRequest();

	// Pido una textura de depth de 24 bits de profundidad para usar posteriormente (no solo para depth evaluation). Mantener el min y mag filter en Nearest!
	TextureRequest* depthRequest = new TextureRequest(screenSize, catalog->getFormatDepth(), catalog->getFormatDepth24());
	depthRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMinFilterFlag(), catalog->getTextureNearestFilterFlag()));
	depthRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMagFilterFlag(), catalog->getTextureNearestFilterFlag()));
	depthRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapSFlag(), catalog->getTextureClampToEdgeFlag()));
	depthRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapTFlag(), catalog->getTextureClampToEdgeFlag()));
	fboRequest->setDepthBufferRequest(depthRequest);

	//Normal
	TextureRequest* normalRequest = new TextureRequest(screenSize, catalog->getFormatRGB(), catalog->getFormatRGB32F());
	normalRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMinFilterFlag(), catalog->getTextureNearestFilterFlag()));
	normalRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMagFilterFlag(), catalog->getTextureNearestFilterFlag()));
	normalRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapSFlag(), catalog->getTextureClampToEdgeFlag()));
	normalRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapTFlag(), catalog->getTextureClampToEdgeFlag()));
	fboRequest->addTextureRequest(normalRequest);

	//Difusa
	TextureRequest* difuseRequest = new TextureRequest(screenSize, catalog->getFormatRGB(), catalog->getFormatRGB8());
	difuseRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMinFilterFlag(), catalog->getTextureNearestFilterFlag()));
	difuseRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMagFilterFlag(), catalog->getTextureNearestFilterFlag()));
	difuseRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapSFlag(), catalog->getTextureClampToEdgeFlag()));
	difuseRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapTFlag(), catalog->getTextureClampToEdgeFlag()));
	fboRequest->addTextureRequest(difuseRequest);

	//Material
	TextureRequest* materialRequest = new TextureRequest(screenSize, catalog->getFormatRGB(), catalog->getFormatRGB8());
	materialRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMinFilterFlag(), catalog->getTextureNearestFilterFlag()));
	materialRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMagFilterFlag(), catalog->getTextureNearestFilterFlag()));
	materialRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapSFlag(), catalog->getTextureClampToEdgeFlag()));
	materialRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapTFlag(), catalog->getTextureClampToEdgeFlag()));
	fboRequest->addTextureRequest(materialRequest);

	return fboRequest;
}
