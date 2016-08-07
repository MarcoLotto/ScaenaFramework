/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "MemoryTextureManager.h"
#include "ApiTexture.h"
#include "DepthTexture.h"
#include "TextureLoadException.h"
#include "GraphicDevice.h"

MemoryTextureManager* MemoryTextureManager::instance = NULL;

MemoryTextureManager* MemoryTextureManager::getInstance(){
	if(instance == NULL){
		instance = new MemoryTextureManager();
	}
	return instance;
}

void MemoryTextureManager::internalAssignTextureId(unsigned int textureId, MemoryTexture* texture){
	// Creamos la lista de instancias para el id
	map<unsigned int, list<MemoryTexture*>*>::iterator it = this->textureInstances.find(textureId);
	if(it != this->textureInstances.end()){
		throw new TextureLoadException("Se quiso asignar un id de textura que ya estaba asignado (y con instancias) en MemoryTextureManager"); 
	}
	this->textureInstances[textureId] = new list<MemoryTexture*>();
	this->textureInstances[textureId]->push_back(texture);	
}

unsigned int MemoryTextureManager::assignTextureId(ApiTexture* texture){
	// Reservamos espacio en memoria para la textura
	unsigned int textureId = GraphicDevice::getInstance()->generateTexture(texture->getSize(), texture->getInternalFormat(), texture->getExternalFormat(), NULL);
	this->internalAssignTextureId(textureId, texture);	
	return textureId;
}

unsigned int MemoryTextureManager::assignTextureId(DepthTexture* texture){
	// Reservamos espacio en memoria para la textura
	unsigned int textureId = GraphicDevice::getInstance()->generateDepthBuffer(texture->getSize().x, texture->getSize().y);
	this->internalAssignTextureId(textureId, texture);
	return textureId;
}

unsigned int MemoryTextureManager::assignTextureId(MemoryTexture* original, MemoryTexture* clone){
	unsigned int originalTextureId = original->getTextureId();
	map<unsigned int, list<MemoryTexture*>*>::iterator it = this->textureInstances.find(originalTextureId);
	if(it != this->textureInstances.end()){
		(*it).second->push_back(clone);
		return originalTextureId;
	}
	throw new TextureLoadException("Se quiso clonar una MemoryTexture con id inexistente en el MemoryTextureManager"); 
}
	
void MemoryTextureManager::deleteTextureInstance(MemoryTexture* texture){
	unsigned int textureId = texture->getTextureId();
	map<unsigned int, list<MemoryTexture*>*>::iterator it = this->textureInstances.find(textureId);
	if(it == this->textureInstances.end()){
		throw new TextureLoadException("Se quiso borrar una instancia de id inexistente en MemoryTextureManager"); 
	}
	list<MemoryTexture*>* instances = (*it).second;
	this->eraseTextureInstance(instances, texture);

	// Si ya no quedan mas instancias declaradas para el id, borro los recursos de la textura fisicamente
	if(instances->size() == 0){
		GraphicDevice::getInstance()->deleteTexture(it->first);
		delete it->second;
		this->textureInstances.erase(it);
	}
}

void MemoryTextureManager::eraseTextureInstance(list<MemoryTexture*>* instances, MemoryTexture* texture){	
	list<MemoryTexture*>::iterator instanceIt = instances->begin();
	for(; instanceIt != instances->end(); instanceIt++){
		if((*instanceIt) == texture){
			instances->erase(instanceIt);
			return;
		}
	}
	throw new TextureLoadException("MemoryTextureManager: Se intenta borrar una instancia de una textura que no fue declarada");
}