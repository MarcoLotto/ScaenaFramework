/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "TextureRequest.h"
#include <glm/glm.hpp> 
#include <list>
using namespace std;

class FrameBuffer;

typedef glm::vec2 vec2;

class FrameBufferRequest{

private:
	bool requiredDepthBuffer;
	TextureRequest* depthRequest;
	vector<TextureRequest*> textureRequests;
	unsigned int hashCode;

	void addTextureRequestInformationToList(TextureRequest* textureRequest, list<unsigned int>* textureInformation);
	void generateHashCode();

public:
	FrameBufferRequest();
	virtual ~FrameBufferRequest();

	bool isRequiredDepthBuffer(){ return this->requiredDepthBuffer; }
	
	void addTextureRequest(int width, int height, unsigned int format, unsigned int internalFormat);

	void addTextureRequest(TextureRequest* textureRequest);
	vector<TextureRequest*>* getTextureRequests(){ return &this->textureRequests; }

	void setDepthBufferRequest(TextureRequest* depthRequest);
	TextureRequest* getDepthBufferRequest(){ return this->depthRequest; }

	// Evalua si el frame buffer corresponde con el request
	bool matches(FrameBuffer* frameBuffer);

	// Devuelve el hash code del frame buffer request para identificar la composicion del mismo
	unsigned int getHashCode(){ return this->hashCode; }
};