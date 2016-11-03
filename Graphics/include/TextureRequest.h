/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ApiTexture.h"
#include "DepthTexture.h"
#include "TextureFilterRequest.h"
#include <list>
using namespace std;

class TextureRequest{

private:
	int width;
	int height;
	unsigned int format;
	unsigned int internalFormat;
	list<TextureFilterRequest*>* filterRequests;

public:
	TextureRequest(int width, int height, unsigned int format, unsigned int internalFormat);
	TextureRequest(vec2 size, unsigned int format, unsigned int internalFormat);
	virtual ~TextureRequest();

	int getWidth(){ return this->width; }
	int getHeight(){ return this->height; }
	unsigned int getFormat(){ return this->format; }
	unsigned int getInternalFormat(){ return this->internalFormat; }
	
	bool matches(ApiTexture* texture);
	bool matchesDepthTexture(MemoryTexture* texture);

	void addFilterRequest(TextureFilterRequest* request){ this->filterRequests->push_back(request); }
	list<TextureFilterRequest*>* getFiltersRequests(){ return this->filterRequests; }
};