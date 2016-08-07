/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ApiTexture.h"
#include "MemoryTextureManager.h"

ApiTexture::ApiTexture() : MemoryTexture(){
	this->textureValid = false;
}

ApiTexture::ApiTexture(vec2 size, int format, int internalFormat) : MemoryTexture(){
	this->generate(size, format, internalFormat);
}

ApiTexture::ApiTexture(ApiTexture* textureToClone) : MemoryTexture(){
	this->setExternalFormat(textureToClone->getExternalFormat());
	this->setInternalFormat(textureToClone->getInternalFormat());
	this->setSize(textureToClone->getSize());
	this->setWriteLock(textureToClone->hasWriteLock());
	this->assignTextureIdForClone(textureToClone);
}

ApiTexture::~ApiTexture(){
	this->deleteTextureFromMemory();
}

//Genera en memoria la textura, de un tamaño y tipo determinado
void ApiTexture::generate(vec2 size, int format, int internalFormat){	
	this->textureValid = true;
	this->setSize(size);
	this->setInternalFormat(internalFormat);
	this->setExternalFormat(format);

	// Reservo memoria para la nueva textura
	this->textureId = MemoryTextureManager::getInstance()->assignTextureId(this);	
}

void ApiTexture::deleteTextureFromMemory(){
	if(this->textureValid)
		MemoryTextureManager::getInstance()->deleteTextureInstance(this);
}

Texture* ApiTexture::clone(){
	return new ApiTexture(this);
}

void ApiTexture::assignTextureIdForClone(ApiTexture* originalToClone){
	this->textureId = MemoryTextureManager::getInstance()->assignTextureId(originalToClone, this);
	this->textureValid = true;
}