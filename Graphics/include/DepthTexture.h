/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MemoryTexture.h"

class DepthTexture : public MemoryTexture{

protected:
	void deleteTextureFromMemory();
	void generate(vec2 size);
	void assignTextureIdForClone(DepthTexture* originalToClone);
	
public:
	// Dados los parametros de la textura, genera una nueva (con nuevos recursos)
	DepthTexture(vec2 size);

	// Dada una apiTexture, copiaremos sus carateristicas y apuntaremos a sus mismos recursos (no genero nuevos recursos)
	DepthTexture(DepthTexture* textureToClone);

	virtual ~DepthTexture();

	// Devuelve un clon de esta textura, manteniendo los mismos recursos (es decir, sin crear recursos nuevos)
	virtual Texture* clone();
};