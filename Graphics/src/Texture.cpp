/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "GLSLProgram.h"
#include "Texture.h"
#include "GraphicDevice.h"

Texture::Texture(){
	this->textureId = 0;
	this->size = vec2(0.0f);
	this->internalFormat = 0;
	this->textureValid = false;
}

void Texture::assignToShader(GLSLProgram* shaderProgram, GLSLUniform* uniform, unsigned int textureUnit){
	GraphicDevice::getInstance()->setActiveTexture(textureUnit);
	GraphicDevice::getInstance()->bindTexture(this->textureId);
	shaderProgram->setUniform(uniform, (int) textureUnit); 
}
