/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GLSLProgram.h"

class Texture;

typedef glm::vec2 vec2;

class RandomTextureGeneratorShader : public GLSLProgram{

private:
	static RandomTextureGeneratorShader* instance;
	mat4 PVMmatrix;

	GLSLUniform* valueVectorRedUniform;
	GLSLUniform* valueVectorGreenUniform;
	GLSLUniform* valueVectorBlueUniform;
	GLSLUniform* pvmMatrixUniform;
	GLSLUniform* sizeUniform;

protected:
	Mesh* rectangleMesh;
	void initProyectionMatrix();
	float* generateRandomVector(vec2 sizeInPixels, float minRange, float maxRange);
	float random(float a, float b);
		
public:
	static RandomTextureGeneratorShader* getInstance();
	
	virtual void draw(vec2 sizeInPixels, float minRange, float maxRange);
	virtual void init();
};