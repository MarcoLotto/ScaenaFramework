/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <stdlib.h>
#include <string>
using namespace std;

class TextureDataTransfer{

public:
	string filename;
	unsigned int width;
	unsigned int height;
	int numberOfPasses;
	int numberOfChannels;
	int colorType;
	int bitDepth;
	unsigned char* data;

	TextureDataTransfer(){
		this->width = 0;
		this->height = 0;
		this->numberOfChannels = 0;
		this->numberOfPasses = 0;
		this->data = NULL;
	}

	virtual ~TextureDataTransfer(){
		// Borro la data de la textura (en memoria principal, no en video)		
		if(this->data != NULL){
			delete this->data;
			this->data = NULL;
		}
	}
};