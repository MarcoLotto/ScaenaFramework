/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "TextureDataTransfer.h"

class PngTextureLoader{

public:
	// Lee el archivo filename de disco y devuelve toda su informacion en un texture data transfer
	static TextureDataTransfer* loadTexture(string filename);

	// Escribe un textura data transfer en un archivo filename en disco. No se borra el textureDt.
	static void writeTexture(string filename, TextureDataTransfer* textureDt);
};