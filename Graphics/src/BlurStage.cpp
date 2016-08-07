/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "BlurStage.h"
#include "GraphicDevice.h"
#include "FrameBufferManager.h"
#include "VideoConfiguration.h"
#include "BlurShader_330.h"

#define ITERATIONS 3

BlurStage::BlurStage(BackBufferWriterStage* previousStage, int imageWidth, int imageHeight, float blurFactor){
	this->firstFrameBuffer = NULL;
	this->secondFrameBuffer = NULL;
	this->fboRequest1 = this->configureFBORequest(imageWidth, imageHeight);
	this->configureStageOutputs();
	this->previousStage = previousStage;
	this->screenSize = vec2(imageWidth, imageHeight);

	//Propio del blur
	this->weights = new float[ITERATIONS];
	this->posIncrement = new float[ITERATIONS];
	this->changeBlurFactor(blurFactor);	
	this->intiateShaderProgram();
}

void BlurStage::intiateShaderProgram(){
	//Elijo el blur a inicializar
	if(VideoConfiguration::getInstance()->isGlsl400Avaible()){
		this->blurShader = BlurShader::getInstance();
	}
	else{
		this->blurShader = BlurShader_330::getInstance();
	}
}
	
// Actaliza la informacion de las texturas de sus outputs
void BlurStage::configureStageOutputs(){
	this->addOutputResource(new OutputResource(NULL));  // La imagen con blur aplicado
	this->addOutputResource(new OutputResource(NULL));  // La textura auxiliar del blur (debe liberarse)
}

// Renderiza el render stage actual
void BlurStage::render(bool isTheLastStage){
	// Cambio el viewport al corecto segun el tamaño de imagen
	GraphicDevice::getInstance()->setViewport(0,0, this->screenSize.x, this->screenSize.y);

	// Primera pasada
	this->firstFrameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequest1, this->firstFrameBuffer);	
	GraphicDevice::getInstance()->clearColorBuffer();

	Texture* textureToBlur = this->previousStage->getBackBuffer()->getOutputTexture();
	this->blurShader->drawFirstPass(textureToBlur, textureToBlur->getSize(), this->weights, this->posIncrement);
	
	// Segunda pasada
	this->secondFrameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequest1, this->secondFrameBuffer, isTheLastStage);	
	GraphicDevice::getInstance()->clearColorAndDepthBuffer();	

	textureToBlur = this->firstFrameBuffer->getOutputTextures()->at(0);
	this->blurShader->drawSecondPass(textureToBlur, textureToBlur->getSize(), this->weights, this->posIncrement);
}

void BlurStage::updateOutputData(){
	this->updateOutputResource(this->secondFrameBuffer->getOutputTextures()->at(0), 0); // La imagen con blur aplicado
	this->updateOutputResource(this->firstFrameBuffer->getOutputTextures()->at(0), 1); // La textura auxiliar del blur (debe liberarse)
}
