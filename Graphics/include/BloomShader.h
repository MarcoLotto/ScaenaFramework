/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GLSLProgram.h"

class Texture;

typedef glm::vec2 vec2;

#define MAX_ITERATIONS 5

class BloomShader : public GLSLProgram{

private:
	static BloomShader* instance;
	
protected:
	Mesh* rectangleMesh;
	mat4 PVMmatrix;
	
	void initProyectionMatrix();

	GLSLUniform* pvmMatrixUniform;
	GLSLUniform* imageSizeUniform;
	GLSLUniform* textureToBlurUniform;
	GLSLUniform* lumThresholdUniform;
	GLSLUniform* weightUniform;
	GLSLUniform* positionIncrementUniform;
	GLSLUniform* originalTextureUniform;
	
public:
	static BloomShader* getInstance();
	
	virtual void drawFirstPass(Texture* original, vec2 imageSize, float* weights, float* posIncrement, float lumThreshold);
	virtual void drawSecondPass(Texture* auxiliary, vec2 imageSize, float* weights, float* posIncrement, float lumThreshold);
	virtual void drawThirdPass(Texture* original, Texture* auxiliary, vec2 imageSize, float* weights, float* posIncrement, float lumThreshold);

	virtual void init();
};
