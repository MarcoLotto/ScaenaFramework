/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <map>
#include <list>
using namespace std;

class MemoryTexture;
class DepthTexture;
class ApiTexture;

class MemoryTextureManager{

private:
	static MemoryTextureManager* instance;
	map<unsigned int, list<MemoryTexture*>*> textureInstances;

	MemoryTextureManager(){};
	void eraseTextureInstance(list<MemoryTexture*>* instances, MemoryTexture* texture);
	void internalAssignTextureId(unsigned int textureId, MemoryTexture* texture);

public:
	static MemoryTextureManager* getInstance();

	// Le asigna un id nuevo (asignando recursos) a una textura
	unsigned int assignTextureId(ApiTexture* texture);

	// Le asigna un id nuevo para depth (asignando recursos)
	unsigned int assignTextureId(DepthTexture* texture);

	// Dada una textura original y otra copia, retorna el id de textura del original
	unsigned int assignTextureId(MemoryTexture* original, MemoryTexture* clone);

	// Borra de la lista de texturas la textura establecida, y si es la ultima instancia borra los recursos de la misma
	void deleteTextureInstance(MemoryTexture* texture);
};