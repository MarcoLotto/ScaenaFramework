/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ShadowBlurStage.h"
#include "GraphicDevice.h"
#include "FrameBufferManager.h"

ShadowBlurStage::ShadowBlurStage(VarianceShadowMapStage* previousStage, int imageWidth, int imageHeight, float blurFactor) : 
BlurStage(previousStage, imageWidth, imageHeight, blurFactor) {
	this->shadowStage = previousStage;
	this->blurPrebankLoaded = false;
	this->fboRequestPass1 = this->configureShadowFBORequestPass1(imageWidth, imageHeight);
	this->fboRequestPass2 = this->configureShadowFBORequestPass2(imageWidth, imageHeight);
}

ShadowBlurStage::~ShadowBlurStage(){
	delete this->fboRequestPass1;
	delete this->fboRequestPass2;
}

// Renderiza el render stage actual
void ShadowBlurStage::render(bool isTheLastStage){
	bool needToRender = !this->shadowStage->isPrebanckEnabled() || !this->shadowStage->isPrebanckPrepared(); 
	if(needToRender || this->blurPrebankLoaded){
		// Cambio el viewport al corecto segun el tamaño de imagen
		GraphicDevice::getInstance()->setViewport(0,0, this->screenSize.x, this->screenSize.y);

		// Primera pasada
		this->firstFrameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequestPass1, this->firstFrameBuffer);
		GraphicDevice::getInstance()->clearColorBuffer();
	
		Texture* textureToBlur = this->previousStage->getBackBuffer()->getOutputTexture();
		this->blurShader->drawFirstPass(textureToBlur, textureToBlur->getSize(), this->weights, this->posIncrement);

		// Me aseguro que la textura no tenga writelock para poder pedir la misma (por el prebankeado)
		if(this->secondFrameBuffer != NULL){
			this->secondFrameBuffer->getOutputTextures()->at(0)->setWriteLock(false);
		}
	
		// Segunda pasada
		this->secondFrameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequestPass2, this->secondFrameBuffer, isTheLastStage);
		GraphicDevice::getInstance()->clearColorAndDepthBuffer();
	
		textureToBlur = this->firstFrameBuffer->getOutputTextures()->at(0);
		this->blurShader->drawSecondPass(textureToBlur, textureToBlur->getSize(), this->weights, this->posIncrement);

		// Tengo ahora informacion de renderizado
		this->blurPrebankLoaded = true;
	}
	else{
		this->firstFrameBuffer = NULL;
	}
	// Me aseguro que la textura final tenga writelock
	this->secondFrameBuffer->getOutputTextures()->at(0)->setWriteLock(true);

	// Si no está hecho el prebanck de la sombra, tengo que renderizar una vez mas el blur
	if(!this->shadowStage->isPrebanckEnabled()){
		this->blurPrebankLoaded = false;
	}
}

void ShadowBlurStage::updateOutputData(){
	this->updateOutputResource(this->secondFrameBuffer->getOutputTextures()->at(0), 0); // La imagen con blur aplicado
	//Si esta activado el prebankeado, no tengo que actualizar la sombra, por lo que tengo que evitar que me la pisen
	this->allowsUnlockOnOutputResource(!this->shadowStage->isPrebanckEnabled(), 0);
	
	if(this->firstFrameBuffer != NULL){
		this->updateOutputResource(this->firstFrameBuffer->getOutputTextures()->at(0), 1); // La textura auxiliar del blur (debe liberarse)
	}
	else{
		FrameBuffer* defaultFrameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(NULL, NULL, true);
		this->updateOutputResource(defaultFrameBuffer->getOutputTextures()->at(0), 1);
	}
}

FrameBufferRequest* ShadowBlurStage::configureShadowFBORequestPass1(int width, int height){
	GraphicDevice* device = GraphicDevice::getInstance();
	GraphicDeviceConstantCatalog* catalog = device->getConstantCatalog();

	//Genero el FrameBufferRequest
	FrameBufferRequest* request = new FrameBufferRequest();
	
	//Mantener con GL_LINEAR para que los post efectos que vengan despues hagan la mitad de fetchs
	TextureRequest* positionRequest = new TextureRequest(vec2(width, height), catalog->getFormatRG(), catalog->getFormatRG32F());
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMinFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMagFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapSFlag(), catalog->getTextureClampToEdgeFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapTFlag(), catalog->getTextureClampToEdgeFlag()));
	request->addTextureRequest(positionRequest);
	return request;
}

FrameBufferRequest* ShadowBlurStage::configureShadowFBORequestPass2(int width, int height){
	GraphicDevice* device = GraphicDevice::getInstance();
	GraphicDeviceConstantCatalog* catalog = device->getConstantCatalog();

	//Genero el FrameBufferRequest
	FrameBufferRequest* request = new FrameBufferRequest();
	
	//Mantener con GL_LINEAR para que los post efectos que vengan despues hagan la mitad de fetchs
	TextureRequest* positionRequest = new TextureRequest(vec2(width, height), catalog->getFormatRG(), catalog->getFormatRG32F());
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMinFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMagFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapSFlag(), catalog->getTextureClampToBorderFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapTFlag(), catalog->getTextureClampToBorderFlag()));
	float* border= new float[4]();
	border[0] = 255.0f; border[1] = 255.0f;  border[2] = 0.0f;  border[3] = 0.0f; 	
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureBorderColorFlag(), border, 4));
	request->addTextureRequest(positionRequest);
	return request;
}