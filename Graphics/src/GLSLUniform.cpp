/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "GLSLUniform.h"
#include "GLSLProgram.h"

GLSLUniform::GLSLUniform(const char* uniformName, GLSLProgram* glslProgram){
	// Consigo el id del uniform en el programa
	this->uniformLocation = glslProgram->getUniformLocation(uniformName);
	this->glslProgram = glslProgram;
	this->uniformName = uniformName;
}

GLSLUniform::GLSLUniform(const char* uniformName){
	this->uniformLocation = -1;
	this->glslProgram = NULL;
	this->uniformName = uniformName;
}

void GLSLUniform::setGlslProgram(GLSLProgram* program){
	this->glslProgram = program;
	this->uniformLocation = this->glslProgram->getUniformLocation(this->uniformName);
}