/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "TextureRequest.h"


TextureRequest::TextureRequest(int width, int height, unsigned int format, unsigned int internalFormat){
		this->width = width;
		this->height = height;
		this->format = format;
		this->internalFormat = internalFormat;
		this->filterRequests = new list<TextureFilterRequest*>();
}

TextureRequest::TextureRequest(vec2 size, unsigned int format, unsigned int internalFormat){
		this->width =size.x;
		this->height = size.y;
		this->format = format;
		this->internalFormat = internalFormat;
		this->filterRequests = new list<TextureFilterRequest*>();
}

TextureRequest::~TextureRequest(){
	std::list<TextureFilterRequest*>::iterator it = this->filterRequests->begin();
	while(it != this->filterRequests->end()){
		delete (*it);
		++it;
	}
	delete this->filterRequests;
}

bool TextureRequest::matches(ApiTexture* texture){
	if(texture->getSize().x != this->getWidth()){
		return false;
	}
	if(texture->getSize().y != this->getHeight()){
		return false;
	}
	if(texture->getInternalFormat() != this->getInternalFormat()){
		return false;
	}
	if(texture->getExternalFormat() != this->getFormat()){
		return false;
	}
	// Se controla que matcheen los filtros
	std::list<TextureFilterRequest*>::iterator it = this->filterRequests->begin();
	while(it != this->filterRequests->end()){
		if(!(*it)->matches(texture)){
			return false;
		}		
		++it;
	}
	return true;
}

bool TextureRequest::matchesDepthTexture(MemoryTexture* texture){
	// REVIEW: No estoy controlando los filtros, en el caso de que venga depth para textura
	if(texture->getInternalFormat() != this->getInternalFormat()){
		return false;
	}
	if(texture->getInternalFormat() != 0 && texture->getExternalFormat() != this->getFormat()){
		return false;
	}
	if(texture->getSize().x != this->getWidth()){
		return false;
	}
	if(texture->getSize().y != this->getHeight()){
		return false;
	}	
	return true;
}