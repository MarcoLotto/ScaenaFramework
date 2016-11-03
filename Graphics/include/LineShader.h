/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

#include "GLSLProgram.h"
#include "ShaderManager.h"


class LineShader : public GLSLProgram{

private:
	static LineShader* instance;

	GLSLUniform* mvpMatrixUniform;
	GLSLUniform* lineColorUniform;

public:
	static LineShader* getInstance();
	
	void draw(int vaoHandle, mat4 viewMatrix, mat4 projMatrix, vec3 color);
	void init();
};
