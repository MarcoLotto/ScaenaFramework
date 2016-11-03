/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "FrameBufferRequest.h"
#include "FrameBuffer.h"
#include "GenericUtils.h"

FrameBufferRequest::FrameBufferRequest(){
	this->requiredDepthBuffer = false;
	this->depthRequest = NULL;
}
FrameBufferRequest::~FrameBufferRequest(){
	for(int i=0; i < this->textureRequests.size(); i++){
		delete this->textureRequests.at(i);
	}
	this->textureRequests.clear();
	
	if(this->depthRequest != NULL){
		delete depthRequest;
	}
}

void FrameBufferRequest::addTextureRequest(int width, int height, unsigned int format, unsigned int internalFormat){
	this->addTextureRequest(new TextureRequest(width, height, format, internalFormat));
	this->generateHashCode();
}

void FrameBufferRequest::setDepthBufferRequest(TextureRequest* depthRequest){ 
	this->depthRequest = depthRequest; 
	this->requiredDepthBuffer = true;
	this->generateHashCode();
}

void FrameBufferRequest::addTextureRequest(TextureRequest* textureRequest){ 
	this->textureRequests.push_back(textureRequest);
	this->generateHashCode();
}

bool FrameBufferRequest::matches(FrameBuffer* frameBuffer){
	vector<ApiTexture*>* textures = frameBuffer->getOutputTextures();
	//Si la cantidad de texturas no son las mismas, ya no machea
	if(this->textureRequests.size() != textures->size()){
		return false;
	}

	//Verifico si matchea el depth buffer
	MemoryTexture* depthBuffer = frameBuffer->getDepthBuffer();
	if(this->isRequiredDepthBuffer()){
		if(depthBuffer == NULL){
			return false;
		}
		if(!this->depthRequest->matchesDepthTexture(depthBuffer)){
			return false;
		}
	}
	else if(depthBuffer != NULL){
		return false;
	}

	//Verifico si matchea cada una de las texturas de salida
	for(int i=0; i < textures->size(); i++){
		if(!this->textureRequests.at(i)->matches(textures->at(i))){
			return false;
		}
	}
	return true;
}

void FrameBufferRequest::addTextureRequestInformationToList(TextureRequest* textureRequest, list<unsigned int>* textureInformation){
	// ATENCION: Si algo cambia aca, debe cambiar tambien en FrameBuffer
	textureInformation->push_back((unsigned int) textureRequest->getWidth());
	textureInformation->push_back((unsigned int) textureRequest->getHeight());
	textureInformation->push_back((unsigned int) textureRequest->getInternalFormat());
	textureInformation->push_back((unsigned int) textureRequest->getFormat());
}

void FrameBufferRequest::generateHashCode(){
	// ATENCION: Si algo cambia aca, debe cambiar tambien en FrameBuffer
	// Guardo toda la informacion de los request texturas en un vector y mando a generar el codigo de hash
	list<unsigned int> hashValues;
	for(unsigned int i=0; i < this->textureRequests.size(); i++){
		TextureRequest* texRequest = this->textureRequests.at(i);
		this->addTextureRequestInformationToList(texRequest, &hashValues);
	}
	if(this->depthRequest != NULL){
		this->addTextureRequestInformationToList(depthRequest, &hashValues);
	}
	this->hashCode = GenericUtils::generateHashCode(&hashValues);
}