/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "LineShader.h"
#include "GraphicDevice.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/LineShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/LineShader.frag"
//********************************************************************************************************

LineShader* LineShader::instance = NULL;

LineShader* LineShader::getInstance(){
	if(instance==NULL){
		instance = new LineShader();
		instance->init();
	}
	return instance;
}
void LineShader::draw(int vaoHandle, mat4 viewMatrix, mat4 projMatrix, vec3 color){		
	//Activo el programa de shaders
	this->use();

	//Cargo los uniforms
	this->setUniform(this->mvpMatrixUniform, projMatrix*viewMatrix);
	this->setUniform(this->lineColorUniform, color);  //Linea roja
	
	//Mando a renderizar
	GraphicDevice::getInstance()->drawLine(vaoHandle);
}

void LineShader::init(){

	list<string> inputAttributes;
	inputAttributes.push_back("VertexPosition");
		
	list<string> outputAttributes;
	outputAttributes.push_back("NormalData");
	outputAttributes.push_back("ColorData");
	outputAttributes.push_back("MaterialData");

	this->setVertexShaderName(VSHADER);
	this->setFragmentShaderName(FSHADER);
	this->initialize(&inputAttributes, &outputAttributes);	

	// Identifico los uniforms a utilizar
	this->mvpMatrixUniform = new GLSLUniform("MVPmatrix", this);
	this->lineColorUniform = new GLSLUniform("colorDeLinea", this);
}
