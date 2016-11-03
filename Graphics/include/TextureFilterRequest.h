/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "TextureFilter.h"
#include "ApiTexture.h"

class TextureFilterRequest{

private:
	unsigned int type;
	unsigned int value;

	float* valueArray;
	unsigned int valueArrayLength;
	unsigned int valueArrayHashCode;
	
public:
	TextureFilterRequest(unsigned int type, unsigned int value);
	TextureFilterRequest(unsigned int type, float* valueArray, unsigned int arrayLength);
	virtual ~TextureFilterRequest();

	unsigned int getType();
	unsigned int getValue();
	float* getValueArray();
	unsigned int getValueArrayLength();

	// Verifica que la textura tenga el filtro pedido
	bool matches(ApiTexture* texture);
};