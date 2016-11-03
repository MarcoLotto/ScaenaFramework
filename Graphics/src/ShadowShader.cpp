/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ShadowShader.h"
#include "GraphicDevice.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/ShadowShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/ShadowShader.frag"
#define TO_LOWERSHADER_DISTANCE 9999999.0f //Se define la distancia para que cambie el shader a uno mas básico
//********************************************************************************************************

ShadowShader* ShadowShader::instance = NULL;

ShadowShader* ShadowShader::getInstance(){
	if(instance==NULL){
		instance = new ShadowShader();
		instance->init();
	}
	return instance;
}
void ShadowShader::prepareForDraw(Mesh* mesh){
	
	//Activo el programa de shaders
	this->use();

	//Cargo matrices de modelo - view
	this->setUniform(this->pvmMatrixUniform, mesh->getDrawPacket()->getPvmMatrix());
}

void ShadowShader::init(){
		
	list<string> inputAttributes;
	inputAttributes.push_back("VertexPosition");
		
	list<string> outputAttributes;
	outputAttributes.push_back("ShadowData");	

	this->setVertexShaderName(VSHADER);
	this->setFragmentShaderName(FSHADER);
	this->initialize(&inputAttributes, &outputAttributes);

	// Identifico los uniforms a utilizar
	this->pvmMatrixUniform = new GLSLUniform("PVMmatrix", this);
   
	//PARAMATROS DE OPTIMIZACION************************************************
	this->setLowerShader(NULL);  //A que shader pasa al optimizar
	this->setDistanceToChangeShader(TO_LOWERSHADER_DISTANCE); //Distancia en la que se optimizará
	//**************************************************************************
}
