/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Texture.h"
#include "TextureManager.h"

class FileTexture : public Texture{

private:
	static TextureManager textureManager;
	string filename;

public:
	FileTexture();
	FileTexture(const char* filename);
	virtual ~FileTexture();

	//Carga una textura. Parametro 1: Path de la textura.
	void loadFromFile(const char *filename);

	string getFilename(){ return this->filename; }

	virtual Texture* clone();
};