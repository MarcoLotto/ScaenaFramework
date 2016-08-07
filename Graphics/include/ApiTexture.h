/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MemoryTexture.h"

class ApiTexture : public MemoryTexture{

private:
	void deleteTextureFromMemory();
	void generate(vec2 size, int format, int internalFormat);
	void assignTextureIdForClone(ApiTexture* originalToClone);
	
public:
	// Usar este constructor solo para hacer mocks, a la textura no se le crearán recursos
	ApiTexture();

	// Dados los parametros de la textura, genera una nueva (con nuevos recursos)
	ApiTexture(vec2 size, int format, int internalFormat);

	// Dada una apiTexture, copiaremos sus carateristicas y apuntaremos a sus mismos recursos (no genero nuevos recursos)
	ApiTexture(ApiTexture* textureToClone);

	virtual ~ApiTexture();
	
	// Devuelve un clon de esta textura, manteniendo los mismos recursos (es decir, sin crear recursos nuevos)
	virtual Texture* clone();
};