/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Texture.h"
#include "FrameBufferRequest.h"
#include <glm/glm.hpp>
#include <list>
using namespace std;

class ImageComposer{

private:
	struct CompositionPacket{
		Texture* texture;
		vec2 position;
		vec2 scale;
		float transparency;
	};
	list<CompositionPacket*> texturesToCompose;
	vec2 finalImageSize;

	FrameBufferRequest* fboRequest;
	FrameBufferRequest* configureFBORequest(int width, int height);

public:
	ImageComposer(vec2 finalImageSize);
	virtual ~ImageComposer();	

	// Genera la composicion de texturas en una sola y la devuelve. La misma tendra writeLock activado para no ser borrada.
	Texture* makeComposition();

	// Agrega una textura a la composicion. El orden en que se agregan influira en como se dibujaran. 
	// Position y scale son un vec2 entre 0.0f y 1.0f, igual que transparency pero float.
	void addImage(Texture* texture, vec2 position, vec2 scale, float transparency);
};