/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "BloomStage.h"
#include "VideoConfiguration.h"
#include "BloomShader_330.h"
#include "FrameBufferManager.h"
#include "GraphicDevice.h"

#define ITERATIONS 3
#define DEFAULT_LUMTHRESHOLD 0.7f;

BloomStage::BloomStage(BackBufferWriterStage* previousStage, int imageWidth, int imageHeight, float blurFactor){
	this->firstFrameBuffer = NULL;
	this->secondFrameBuffer = NULL;
	this->thirdFrameBuffer = NULL;
	this->fboRequest1 = this->configureFBORequest(imageWidth, imageHeight);
	this->configureStageOutputs();
	this->previousStage = previousStage;
	this->lumThreshold = DEFAULT_LUMTHRESHOLD;
	this->screenSize = vec2(imageWidth, imageHeight);

	//Propio del bloom (y blur)
	this->weights = new float[ITERATIONS];
	this->posIncrement = new float[ITERATIONS];
	this->changeBlurFactor(blurFactor);
	this->intiateShaderProgram();
}

void BloomStage::intiateShaderProgram(){
	//Elijo el bloom shader a inicializar
	if(VideoConfiguration::getInstance()->isGlsl400Avaible()){
		this->bloomShader = BloomShader::getInstance();
	}
	else{
		this->bloomShader = BloomShader_330::getInstance();
	}
}

// Actaliza la informacion de las texturas de sus outputs
void BloomStage::configureStageOutputs(){
	this->addOutputResource(new OutputResource(NULL));  // La imagen con bloom aplicado
	this->addOutputResource(new OutputResource(NULL));  // La primera textura auxiliar del bloom (debe liberarse)
	this->addOutputResource(new OutputResource(NULL));  // La segunda textura auxiliar del bloom (debe liberarse)
}

// Renderiza el render stage actual
void BloomStage::render(bool isTheLastStage){
	// Cambio el viewport al corecto segun el tamaño de imagen
	GraphicDevice::getInstance()->setViewport(0,0, this->screenSize.x, this->screenSize.y);

	// Primera pasada (seleccion de pixels a hacer blur)
	this->firstFrameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequest1, this->firstFrameBuffer);
	GraphicDevice::getInstance()->clearColorBuffer();

	Texture* textureToBloom = this->previousStage->getBackBuffer()->getOutputTexture();
	this->bloomShader->drawFirstPass(textureToBloom, textureToBloom->getSize(), this->weights, this->posIncrement, this->lumThreshold);
	
	// Segunda pasada (blur horizontal sobre los pixels seleccionados)
	this->secondFrameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequest1, this->secondFrameBuffer);
	GraphicDevice::getInstance()->clearColorBuffer();
	Texture* auxiliary = this->firstFrameBuffer->getOutputTextures()->at(0);
	this->bloomShader->drawSecondPass(auxiliary, textureToBloom->getSize(), this->weights, this->posIncrement, this->lumThreshold);

	// Tercera pasada (blur vertical y merge de la imagen original con la auxiliar)
	this->thirdFrameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequest1, this->thirdFrameBuffer, isTheLastStage);	
	GraphicDevice::getInstance()->clearColorAndDepthBuffer();
	textureToBloom = this->previousStage->getBackBuffer()->getOutputTexture();
	auxiliary = this->secondFrameBuffer->getOutputTextures()->at(0);
	this->bloomShader->drawThirdPass(textureToBloom, auxiliary, textureToBloom->getSize(), this->weights, this->posIncrement, this->lumThreshold);
}

void BloomStage::updateOutputData(){
	this->updateOutputResource(this->thirdFrameBuffer->getOutputTextures()->at(0), 0); // La imagen con bloom aplicado
	this->updateOutputResource(this->secondFrameBuffer->getOutputTextures()->at(0), 1); // La primera textura auxiliar del bloom (debe liberarse)
	this->updateOutputResource(this->firstFrameBuffer->getOutputTextures()->at(0), 2); // La segunda textura auxiliar del bloom (debe liberarse)
}