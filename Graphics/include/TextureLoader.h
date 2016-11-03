/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "TextureDataTransfer.h"
#include <glm/glm.hpp> 
typedef glm::vec2 vec2;

class TextureLoader{

private:
	vec2 lastTextureSize;
	string lastFilename;

	string getExtension(const char *filename);	
	TextureDataTransfer* parseTextureFile(const char* filename);

protected:
	void applyTextureFilters(TextureDataTransfer* textureData);
	vec2 textureSize;
	int textureInternalFormat;

public:
	virtual unsigned int parseFileAndLoadInMemory(const char* filename);
	virtual vec2 getTextureSize(const char* filename);

	vec2 getTextureSize(){ return this->textureSize; }
	int getTextureInternalFormat(){ return this->textureInternalFormat; }
};