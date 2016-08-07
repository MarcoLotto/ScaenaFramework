/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "LightUniform.h"
#include <stdio.h>

LightUniform::LightUniform(int lightNumber, GLSLProgram* glslProgram){
	char* nombreUniform = new char[30]();
	sprintf(nombreUniform, "luz[%d].posicion", lightNumber);
	this->positionUniform = new GLSLUniform(nombreUniform, glslProgram);
	sprintf(nombreUniform, "luz[%d].direccion", lightNumber);
	this->directionUniform = new GLSLUniform(nombreUniform, glslProgram);
	sprintf(nombreUniform, "luz[%d].intensidad", lightNumber);
	this->intensityUniform = new GLSLUniform(nombreUniform, glslProgram);
	sprintf(nombreUniform, "luz[%d].spotFactor", lightNumber);
	this->spotFactorUniform = new GLSLUniform(nombreUniform, glslProgram);
	sprintf(nombreUniform, "shadowMap%d", lightNumber);
	this->shadowMapUniform = new GLSLUniform(nombreUniform, glslProgram);
	sprintf(nombreUniform, "shadowMatrix%d", lightNumber);
	this->shadowMatrixUniform = new GLSLUniform(nombreUniform, glslProgram);
	sprintf(nombreUniform, "luz[%d].shadowIndex", lightNumber);
	this->shadowIndexUniform = new GLSLUniform(nombreUniform, glslProgram);
}
LightUniform::~LightUniform(){
	delete this->positionUniform;
	delete this->directionUniform;
	delete this->intensityUniform;
	delete this->spotFactorUniform;
	delete this->shadowMapUniform;
	delete this->shadowMatrixUniform;
	delete this->shadowIndexUniform;
}