/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "TextureManager.h"
#include "GLSLUniform.h"
#include <glm/glm.hpp> 
typedef glm::vec2 vec2;

//Antes de incluir esta libreria, tiene que incluirse GLSLProgram.h
class GLSLProgram;

class Texture{

protected:
	unsigned int textureId;	
	vec2 size;
	int internalFormat;
	int externalFormat;
	bool textureValid;

public:	
	Texture();	
	virtual ~Texture(){};

	//Asigna la textura a un programa de shaders. El primer parametro es el programa, el 
	//segundo el nombre de la variable uniform a utilizar.
	void assignToShader(GLSLProgram* shaderProgram, GLSLUniform* uniform, unsigned int textureUnit);
		
	vec2 getSize(){ return this->size; };
	void setSize(vec2 size){ this->size = size; }

	int getInternalFormat(){ return this->internalFormat; }
	void setInternalFormat(int format){ this->internalFormat = format; }

	int getExternalFormat(){ return this->externalFormat; }
	void setExternalFormat(int format){ this->externalFormat = format; }

	bool isTextureValid(){ return this->textureValid; }

	virtual Texture* clone() = 0;
};