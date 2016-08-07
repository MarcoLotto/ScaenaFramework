/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

class GLSLProgram;

class GLSLUniform{

private:
	int uniformLocation;
	GLSLProgram* glslProgram;
	const char* uniformName;
	
public:
	GLSLUniform(const char* uniformName);
	GLSLUniform(const char* uniformName, GLSLProgram* glslProgram);

	// Devuelve la location del uniform en este programa de shader
	int getUniformLocation(){ return this->uniformLocation; }

	// El programa al que pertenece el uniform
	GLSLProgram* getGlslProgram(){ return this->glslProgram; }
	void setGlslProgram(GLSLProgram* program);
};