/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "DepthOfFieldStage.h"
#include "GraphicDevice.h"
#include "FrameBufferManager.h"
#include "VideoConfiguration.h"
#include "FeatureNotAvaibleException.h"

#define ITERATIONS 3
#define DEFAULT_BLUR_FALLOFF 0.03f

DepthOfFieldStage::DepthOfFieldStage(GeometryStage* geometryStage, BackBufferWriterStage* previousStage, int imageWidth, int imageHeight, float blurFactor, 
								float depthAtBlurStart, bool blurToFront){
	this->firstFrameBuffer = NULL;
	this->secondFrameBuffer = NULL;
	this->fboRequest1 = this->configureFBORequest(imageWidth, imageHeight);
	this->configureStageOutputs();
	this->previousStage = previousStage;
	this->geometryStage = geometryStage;
	this->screenSize = vec2(imageWidth, imageHeight);
	this->depthAtBlurStart = depthAtBlurStart;
	this->blurToFront = blurToFront;
	this->blurFallOff = DEFAULT_BLUR_FALLOFF;

	//Propio del blur
	this->weights = new float[ITERATIONS];
	this->posIncrement = new float[ITERATIONS];
	this->changeBlurFactor(blurFactor);	
	this->intiateShaderProgram();
}

void DepthOfFieldStage::intiateShaderProgram(){
	//Elijo el blur a inicializar
	if(VideoConfiguration::getInstance()->isGlsl400Avaible()){
		this->depthOfFieldShader = DepthOfFieldShader::getInstance();
	}
	else{
		throw new FeatureNotAvaibleException("La caracteristica de depth of blur no esta disponible para su hardware actual");
	}
}

// Agrega al vector de inputs los inputs de este Render Stage particular
void DepthOfFieldStage::addStageInputs(list<OutputResource*>* inputs){
	inputs->push_back(this->previousStage->getBackBuffer());
	inputs->push_back(this->geometryStage->getDepthBuffer());
}
	
// Actaliza la informacion de las texturas de sus outputs
void DepthOfFieldStage::configureStageOutputs(){
	this->addOutputResource(new OutputResource(NULL));  // La imagen con blur aplicado
	this->addOutputResource(new OutputResource(NULL));  // La textura auxiliar del blur (debe liberarse)
}

// Renderiza el render stage actual
void DepthOfFieldStage::render(bool isTheLastStage){
	// Cambio el viewport al corecto segun el tamaño de imagen
	GraphicDevice::getInstance()->setViewport(0,0, this->screenSize.x, this->screenSize.y);

	// Primera pasada
	this->firstFrameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequest1, this->firstFrameBuffer);
	GraphicDevice::getInstance()->clearColorBuffer();

	Texture* textureToBlur = this->previousStage->getBackBuffer()->getOutputTexture();
	Texture* depthTexture = this->geometryStage->getDepthBuffer()->getOutputTexture();
	mat4 sceneProj = this->geometryStage->getScene()->getCurrentCamera()->getPerspectiveMatrix();
	this->depthOfFieldShader->drawFirstPass(textureToBlur, depthTexture, textureToBlur->getSize(), this->weights, this->posIncrement, this->depthAtBlurStart, 
		this->blurToFront, sceneProj, this->blurFallOff);
	
	// Segunda pasada
	this->secondFrameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequest1, this->secondFrameBuffer, isTheLastStage);
	GraphicDevice::getInstance()->clearColorAndDepthBuffer();

	textureToBlur = this->firstFrameBuffer->getOutputTextures()->at(0);
	this->depthOfFieldShader->drawSecondPass(textureToBlur, depthTexture, textureToBlur->getSize(), this->weights, this->posIncrement, this->depthAtBlurStart, 
		this->blurToFront, sceneProj, this->blurFallOff);
}

void DepthOfFieldStage::updateOutputData(){
	this->updateOutputResource(this->secondFrameBuffer->getOutputTextures()->at(0), 0); // La imagen con blur aplicado
	this->updateOutputResource(this->firstFrameBuffer->getOutputTextures()->at(0), 1); // La textura auxiliar del blur (debe liberarse)
}
