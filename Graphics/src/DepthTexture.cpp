/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "DepthTexture.h"
#include "MemoryTextureManager.h"

DepthTexture::DepthTexture(vec2 size) : MemoryTexture(){
	this->generate(size);
}

DepthTexture::DepthTexture(DepthTexture* textureToClone) : MemoryTexture(){
	this->setExternalFormat(textureToClone->getExternalFormat());
	this->setInternalFormat(textureToClone->getInternalFormat());
	this->setSize(textureToClone->getSize());
	this->setWriteLock(textureToClone->hasWriteLock());
	this->assignTextureIdForClone(textureToClone);
}

DepthTexture::~DepthTexture(){
	this->deleteTextureFromMemory();
}

//Genera en memoria la textura, de un tamaño y tipo determinado
void DepthTexture::generate(vec2 size){
	this->textureValid = true;
	this->setSize(size);
	this->setInternalFormat(NULL);
	this->setExternalFormat(NULL);

	// Reservo memoria para la nueva textura
	this->textureId = MemoryTextureManager::getInstance()->assignTextureId(this);	
}

void DepthTexture::deleteTextureFromMemory(){
	MemoryTextureManager::getInstance()->deleteTextureInstance(this);
}

Texture* DepthTexture::clone(){
	return new DepthTexture(this);
}

void DepthTexture::assignTextureIdForClone(DepthTexture* originalToClone){
	this->textureId = MemoryTextureManager::getInstance()->assignTextureId(originalToClone, this);
	this->textureValid = true;
}