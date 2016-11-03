/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "FileTexture.h"

TextureManager FileTexture::textureManager = TextureManager();

FileTexture::FileTexture() : Texture(){
}

FileTexture::FileTexture(const char* filename) : Texture(){
	this->loadFromFile(filename);
}

void FileTexture::loadFromFile(const char *filename){
	this->textureValid = true;
	this->filename = filename;
	this->textureId = this->textureManager.getTextureId(filename);
	
	//Guardo los datos
	this->setSize(this->textureManager.getTextureSize());
	this->setExternalFormat(this->textureManager.getTextureInternalFormat());
	this->setInternalFormat(this->textureManager.getTextureInternalFormat());
}

FileTexture::~FileTexture(){
	if(this->isTextureValid()){
		this->textureManager.onTextureDelete(this->filename.c_str());
		this->textureValid = false;
	}
}

Texture* FileTexture::clone(){
	return new FileTexture(this->filename.c_str());
}