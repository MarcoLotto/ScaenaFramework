/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GLSLProgram.h"

class Texture;

typedef glm::vec2 vec2;

// Las operaciones posibles
#define PIXEL_AVERAGE_OPERATION 0
#define PIXEL_MULTIPLICATION_OPERATION 1

class MergeTwoImagesShader : public GLSLProgram{

private:
	static MergeTwoImagesShader* instance;
	mat4 PVMmatrix;
	MergeTwoImagesShader(){};

	GLSLUniform* pvmMatrixUniform;
	GLSLUniform* transparencyUniform;
	GLSLUniform* image1Uniform;
	GLSLUniform* image2Uniform;
	GLSLUniform* operationUniform;

protected:
	Mesh* rectangleMesh;
	void initProyectionMatrix();
	
public:
	static MergeTwoImagesShader* getInstance();
	
	virtual void draw(Texture* image1, Texture* image2, float transparency, int operation);
	virtual void init();
};