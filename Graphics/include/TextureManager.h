/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Texture.h"
#include "TextureLoader.h"

#include <string>
#include <map>
#include <vector>
using namespace std;

typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

class TextureManager{

private:
	//Estructura para almacenar la cantidad de instancias creadas de una textura y su prototipo
	struct AllocatedTexture{
		unsigned int instanceCount;
		unsigned int textureId;
		vec2 size;
		int internalFormat;
		int externalFormat;
	};

	//Atributos privados
	unsigned int cantIds;
	map<string, AllocatedTexture> textureMap;
	TextureLoader* textureLoader;

	vec2 textureSize;
	int textureInternalFormat;

	//Metodos privados
	void configureTextureLoader();  //Configura que implementacion o libreria de loader de texturas usar (ej. QT)
	void assignId(string filename, unsigned int textureId);
	void deleteTexture(unsigned int id);	

	unsigned int allocateTextureMemoryForId(string filename);
	
public:
	TextureManager();	
	unsigned int getTextureId(string filename);
	void onTextureDelete(string filename);

	vec2 getTextureSize(){ return this->textureSize; }
	int getTextureInternalFormat(){ return this->textureInternalFormat; }
};
