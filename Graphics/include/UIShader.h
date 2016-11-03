/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GLSLProgram.h"

class Texture;

typedef glm::vec2 vec2;

class UIShader : public GLSLProgram{

private:
	Mesh* rectangleMesh;
	Mesh* sideCenteredRectangleMesh;
	mat4 PVmatrix;
	static UIShader* instance;
	
	void initProyectionMatrix();

	GLSLUniform* pvmMatrixUniform;
	GLSLUniform* partialSizeUniform;
	GLSLUniform* positionIndexUniform;
	GLSLUniform* transparencyUniform;
	GLSLUniform* textureDataUniform;
	
public:
	static UIShader* getInstance();
	
	void draw(Texture* textureToShow, vec2 partialSize, vec2 positionIndex, mat4 modelMatrix, float transparency);
	void drawSideCenteredCuad(Texture* textureToShow, vec2 partialSize, vec2 positionIndex, mat4 modelMatrix, float transparency);
	void init();
};
