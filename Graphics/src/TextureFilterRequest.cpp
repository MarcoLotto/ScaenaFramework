/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "TextureFilterRequest.h"
#include <stdlib.h>

TextureFilterRequest::TextureFilterRequest(unsigned int type, unsigned int value){
	this->type = type;
	this->value = value;
	this->valueArray = NULL;
	this->valueArrayLength = 0;
	this->valueArrayHashCode = 0;
}
TextureFilterRequest::TextureFilterRequest(unsigned int type, float* valueArray, unsigned int arrayLength){
	this->type = type;
	this->value = 0;
	this->valueArray = valueArray;
	this->valueArrayLength = arrayLength;
	this->valueArrayHashCode = MemoryTexture::generateValueArrayHashCode(valueArray, arrayLength);
}

TextureFilterRequest::~TextureFilterRequest(){
	if(this->valueArray != NULL){
		delete this->valueArray;
		this->valueArray = NULL;
	}
}

unsigned int TextureFilterRequest::getType(){ return this->type; }
unsigned int TextureFilterRequest::getValue(){ return this->value; }
float* TextureFilterRequest::getValueArray(){ return this->valueArray; }
unsigned int TextureFilterRequest::getValueArrayLength(){ return this->valueArrayLength; }

// Verifica que la textura tenga el filtro pedido
bool TextureFilterRequest::matches(ApiTexture* texture){
	if(!texture->isFilterTypeApplied(this->getType())){
		return false;
	}
	unsigned int textureFilter = texture->getAppliedFilter(this->getType());
	// Primero trato que matchee con el array, sino que matchee con el valor simple
	if(this->valueArray != NULL){
		if(this->valueArrayHashCode != textureFilter){
			return false;
		}
	}
	else if(this->getValue() != textureFilter){
		return false;
	}
	return true;
}

