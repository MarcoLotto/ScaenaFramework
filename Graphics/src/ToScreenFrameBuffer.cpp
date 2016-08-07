/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ToScreenFrameBuffer.h"
#include "GraphicDevice.h"
#include "OutputResource.h"
#include "FrameBufferManager.h"

void ToScreenFrameBuffer::createToScreenFrameBuffer(){
	// Asigno una misma textura, vacia, pero muchas veces. Esto es para emular el comportamiento
	// de las texturas en los FrameBuffers que no van a pantalla.
	this->mockTexture = new ApiTexture();
	for(int i=0; i < 15; i++){
		this->outputTextures->push_back(this->mockTexture);
	}
	// Tambien lo hago para la textura de depth
	this->attachDepthBuffer(this->mockTexture);
}

ToScreenFrameBuffer::ToScreenFrameBuffer(){
	this->fboHandler = 0;  // Cero porque va a la pantalla
	this->outputTextures = new vector<ApiTexture*>();
	this->depthBuffer = NULL;
	this->usedTimeSlot = TimeManager::getInstance()->reservTimeSlot();
	this->createToScreenFrameBuffer();
}

ToScreenFrameBuffer::~ToScreenFrameBuffer(){
	delete this->mockTexture;
}

void ToScreenFrameBuffer::bind(){
	GraphicDevice::getInstance()->bindFrameBuffer(this->fboHandler);
	TimeManager::getInstance()->updateTimeSlot(this->usedTimeSlot);

	// Le informo al frame buffer manager que hay un nuevo frame buffer bindeado
	FrameBufferManager::getInstance()->informBindedFrameBuffer(this);
}

void ToScreenFrameBuffer::bindAndLockOutputs(){
	this->bind();
}
