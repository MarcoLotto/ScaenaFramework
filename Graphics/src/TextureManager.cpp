/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include <glm/glm.hpp> 

#include "TextureManager.h"
#include "TextureLoader.h"
#include "GraphicDevice.h"

#include <string>
using namespace std;

TextureManager::TextureManager(){	
	this->configureTextureLoader();
}

void TextureManager::configureTextureLoader(){
	this->textureLoader = new TextureLoader();
}

unsigned int TextureManager::getTextureId(string filename){
	map<string, AllocatedTexture>::iterator it = this->textureMap.find(filename);	
	if(it != this->textureMap.end())
	{
		//Elemento encontrado, incremento la cantidad de instancias y devuelvo
		it->second.instanceCount++;
		this->textureSize = it->second.size;
		this->textureInternalFormat = it->second.internalFormat;
		return it->second.textureId;	
	}
	//Elemento no encontrado, consigo un nuevo id
	unsigned int textureId = this->allocateTextureMemoryForId(filename);	
	//Lo guardo en el mapa para no asignar memoria siempre
	this->assignId(filename, textureId);
	return textureId;	
}

void TextureManager::assignId(string nombre, unsigned int id){
	AllocatedTexture allocatedTexture;
	allocatedTexture.instanceCount = 1;
	allocatedTexture.textureId = id;
	allocatedTexture.size = this->textureSize;
	allocatedTexture.internalFormat = this->textureInternalFormat;
	this->textureMap.insert(std::pair<string, AllocatedTexture>(nombre, allocatedTexture));
}

void TextureManager::onTextureDelete(string filename){
	if(this->textureMap.size() == 0) return;

	map<string, AllocatedTexture>::iterator it = this->textureMap.find(filename);	
	if(it != this->textureMap.end())
	{	
		//Identifico que hay una instancia menos
		it->second.instanceCount--;	
		//Si no hay mas instancias, borro definitivamente la textura de memoria
		if(it->second.instanceCount == 0){
			this->deleteTexture(it->second.textureId);			
			this->textureMap.erase(it);	
			return;
		}			
	}	
}

void TextureManager::deleteTexture(unsigned int id){
	//Borro fisicamente la textura de memoria
	GraphicDevice::getInstance()->deleteTexture(id);
}

unsigned int TextureManager::allocateTextureMemoryForId(string filename){
	//Delego la carga de la textura en el TextureLoader
	unsigned int textureLocation = this->textureLoader->parseFileAndLoadInMemory(filename.c_str());
	this->textureSize = this->textureLoader->getTextureSize();
	this->textureInternalFormat = this->textureLoader->getTextureInternalFormat();
	return textureLocation;
}
