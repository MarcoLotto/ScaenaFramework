/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "FrameBuffer.h"
#include "GraphicDevice.h"
#include "FrameBufferWriteLockViolationException.h"
#include "VideoConfiguration.h"
#include "GenericUtils.h"
#include "FrameBufferManager.h"

FrameBuffer::FrameBuffer(){
	this->outputTextures = new vector<ApiTexture*>();
	this->fboHandler = GraphicDevice::getInstance()->generateFrameBuffer();
	this->depthBuffer = NULL;
	this->usedTimeSlot = TimeManager::getInstance()->reservTimeSlot();
	this->hashCode = 0;
}

FrameBuffer::~FrameBuffer(){
	GraphicDevice::getInstance()->deleteFrameBuffer(this->fboHandler);
	delete this->outputTextures;
	TimeManager::getInstance()->removeTimeSlot(this->usedTimeSlot);
}

void FrameBuffer::bind(){	
	GraphicDevice::getInstance()->bindFrameBuffer(this->fboHandler);
	TimeManager::getInstance()->updateTimeSlot(this->usedTimeSlot);

	//Seteo el fbo para escribir
	GraphicDevice::getInstance()->drawFrameBuffer(this->fboHandler, this->outputTextures->size());

	// Le informo al frame buffer manager que hay un nuevo frame buffer bindeado
	FrameBufferManager::getInstance()->informBindedFrameBuffer(this);
}

void FrameBuffer::bindAndLockOutputs(){
	this->lockOutputTextures();
	this->bind();
}

vector<ApiTexture*>* FrameBuffer::getOutputTextures(){
	return this->outputTextures;
}

void FrameBuffer::lockOutputTextures(){
	// Lockeo el depth buffer y las texturas de output
	if(this->depthBuffer != NULL && this->depthBuffer->getInternalFormat() != 0){
		this->depthBuffer->setWriteLock(true);
	}
	for(int i=0; i < this->outputTextures->size(); i++){
		this->outputTextures->at(i)->setWriteLock(true);
	}
}

void FrameBuffer::addOutputTexture(ApiTexture* texture){
	GraphicDevice* device = GraphicDevice::getInstance();
	device->bindTextureToFrameBuffer(texture, this->fboHandler, this->outputTextures->size());
	this->outputTextures->push_back(texture);
	// Como agregue una nueva textura, vuelvo a generar el hash code
	this->generateHashCode();
}

void FrameBuffer::attachDepthBuffer(DepthTexture* depthBuffer){
	GraphicDevice* device = GraphicDevice::getInstance();
	this->depthBuffer = depthBuffer;
	device->attachDepthBufferToFrameBufferAsRenderBuffer(depthBuffer->getTextureId(), this->fboHandler);
	// Como agregue una nueva textura, vuelvo a generar el hash code
	this->generateHashCode();
}

void FrameBuffer::attachDepthBuffer(ApiTexture* depthBuffer){
	GraphicDevice* device = GraphicDevice::getInstance();
	this->depthBuffer = depthBuffer;
	device->attachDepthBufferToFrameBufferAsTexture(depthBuffer->getTextureId(), this->fboHandler);
	// Como agregue una nueva textura, vuelvo a generar el hash code
	this->generateHashCode();
}

bool FrameBuffer::hasWriteLock(){
	for(int i=0; i < this->outputTextures->size(); i++){
		if(this->outputTextures->at(i)->hasWriteLock()){
			return true;
		}
	}
	return false;
}

void FrameBuffer::setAsScreenFrameBuffer(){
	GraphicDevice::getInstance()->deleteFrameBuffer(this->fboHandler);
	this->fboHandler = 0;
	// Como no puedo generar un hash code porque no tengo texturas, defino un codigo distinto al inicial (0)
	this->hashCode = 1;
}

unsigned int FrameBuffer::getElapsedTimeSinceBinding(){
	return TimeManager::getInstance()->getElapsedTime(this->usedTimeSlot);
}

void FrameBuffer::addTextureInformationToList(MemoryTexture* texture, list<unsigned int>* textureInformation){
	// ATENCION: Si algo cambia aca, debe cambiar tambien en FrameBufferRequest
	textureInformation->push_back((unsigned int) texture->getSize().x);
	textureInformation->push_back((unsigned int) texture->getSize().y);
	textureInformation->push_back((unsigned int) texture->getInternalFormat());
	textureInformation->push_back((unsigned int) texture->getExternalFormat());
}

void FrameBuffer::generateHashCode(){
	// ATENCION: Si algo cambia aca, debe cambiar tambien en FrameBufferRequest
	// Guardo toda la informacion de las texturas en un vector y mando a generar el codigo de hash
	list<unsigned int> hashValues;
	for(unsigned int i=0; i < this->outputTextures->size(); i++){
		ApiTexture* outputTexture = this->outputTextures->at(i);
		this->addTextureInformationToList(outputTexture, &hashValues);
	}
	if(this->depthBuffer != NULL){
		this->addTextureInformationToList(depthBuffer, &hashValues);
	}
	this->hashCode = GenericUtils::generateHashCode(&hashValues);
}

