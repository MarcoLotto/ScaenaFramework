/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "BlurShader.h"

class Texture;

typedef glm::vec2 vec2;

class BlurShader_330 : public BlurShader{

private:
	static BlurShader_330* instance;

	GLSLUniform* pvmMatrixUniform;
	GLSLUniform* imageSizeUniform;
	GLSLUniform* textureToBlurUniform;	
	GLSLUniform* weightUniform;
	GLSLUniform* positionIncrementUniform;
	GLSLUniform* passUniform;

public:
	static BlurShader_330* getInstance();
	
	void drawFirstPass(Texture* textureToBlur, vec2 imageSize, float* weights, float* posIncrement);
	void drawSecondPass(Texture* textureToBlur, vec2 imageSize, float* weights, float* posIncrement);
	void init();
};
