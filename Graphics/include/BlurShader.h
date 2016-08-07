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

class BlurShader : public GLSLProgram{

private:
	static BlurShader* instance;

protected:
	Mesh* rectangleMesh;
	mat4 PVMmatrix;
	float weights[MAX_ITERATIONS];
	float sumOfWeightsPerIteration[MAX_ITERATIONS];

	void initProyectionMatrix();

	GLSLUniform* pvmMatrixUniform;
	GLSLUniform* imageSizeUniform;
	GLSLUniform* textureToBlurUniform;	
	GLSLUniform* weightUniform;
	GLSLUniform* positionIncrementUniform;	
	
public:
	static BlurShader* getInstance();
	
	virtual void drawFirstPass(Texture* textureToBlur, vec2 imageSize, float* weights, float* posIncrement);
	virtual void drawSecondPass(Texture* textureToBlur, vec2 imageSize, float* weights, float* posIncrement);
	virtual void init();
};
