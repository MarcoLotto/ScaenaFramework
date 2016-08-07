/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "BloomShader.h"

class BloomShader_330 : public BloomShader{

private:
	static BloomShader_330* instance;

	GLSLUniform* pvmMatrixUniform;
	GLSLUniform* imageSizeUniform;
	GLSLUniform* textureToBlurUniform;
	GLSLUniform* lumThresholdUniform;
	GLSLUniform* weightUniform;
	GLSLUniform* positionIncrementUniform;
	GLSLUniform* passUniform;
	GLSLUniform* originalTextureUniform;
		
public:
	static BloomShader_330* getInstance();
		
	void drawFirstPass(Texture* original, vec2 imageSize, float* weights, float* posIncrement, float lumThreshold);
	void drawSecondPass(Texture* auxiliary, vec2 imageSize, float* weights, float* posIncrement, float lumThreshold);
	void drawThirdPass(Texture* original, Texture* auxiliary, vec2 imageSize, float* weights, float* posIncrement, float lumThreshold);

	void init();
};
