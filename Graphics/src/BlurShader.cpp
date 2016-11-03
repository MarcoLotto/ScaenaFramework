/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "BlurShader.h"
#include "MeshFactory.h"
#include "VideoConfiguration.h"
#include "GraphicDevice.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/BlurShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/BlurShader.frag"
#define CANT_VERTICES 4
#define ITERATIONS 3
//********************************************************************************************************

BlurShader* BlurShader::instance = NULL;

BlurShader* BlurShader::getInstance(){
	if(instance==NULL){
		instance = new BlurShader();
		instance->init();
		instance->initProyectionMatrix();
		instance->rectangleMesh = MeshFactory::getInstance()->createRectangleMesh();
	}
	return instance;
}

void BlurShader::initProyectionMatrix(){
	glm::vec3 escalaImagen = glm::vec3(2.0f, -2.0f, -2.0f);
	glm::mat4 proyeccionOrtogonal = glm::mat4(1.0f);
	proyeccionOrtogonal[2][2] = 0.0f;	
	glm::mat4 model = glm::scale(glm::mat4(1.0f), escalaImagen);
	this->PVMmatrix = proyeccionOrtogonal * model;
}

void BlurShader::drawFirstPass(Texture* textureToBlur, vec2 imageSize, float* weights, float* posIncrement){

	//Activo el programa de shaders
	this->use();
	GraphicDevice* graphicDevice = GraphicDevice::getInstance();

	//Cargo matrices de modelo, view, proyeccion y normal
	this->setUniform(this->pvmMatrixUniform, this->PVMmatrix);
	this->setUniform(this->imageSizeUniform, imageSize);
	textureToBlur->assignToShader(this, this->textureToBlurUniform, 0);
		
	this->setVectorUniform(this->weightUniform, ITERATIONS, weights);
	this->setVectorUniform(this->positionIncrementUniform, ITERATIONS, posIncrement);

	//Activo la primer subrutina
	this->changeSubroutineInFragmentShader("pass1");

	//Mando a renderizar la primer pasada
	graphicDevice->drawMesh(this->rectangleMesh);
}

void BlurShader::drawSecondPass(Texture* textureToBlur, vec2 imageSize, float* weights, float* posIncrement){

	//Activo el programa de shaders
	this->use();
	GraphicDevice* graphicDevice = GraphicDevice::getInstance();

	//Cargo matrices de modelo, view, proyeccion y normal
	this->setUniform(this->pvmMatrixUniform, this->PVMmatrix);
	this->setUniform(this->imageSizeUniform, imageSize);
	textureToBlur->assignToShader(this, this->textureToBlurUniform, 0);
		
	this->setVectorUniform(this->weightUniform, ITERATIONS, weights);
	this->setVectorUniform(this->positionIncrementUniform, ITERATIONS, posIncrement);

	//Activo la segunda subrutina
	this->changeSubroutineInFragmentShader("pass2");

	//Mando a renderizar la segunda pasada
	graphicDevice->drawMesh(this->rectangleMesh);
}

void BlurShader::init(){
		
	list<string> inputAttributes;
	inputAttributes.push_back("VertexPosition");
	inputAttributes.push_back("TextureCoords");
	
	list<string> outputAttributes;
	outputAttributes.push_back("BlurredData");

	this->setVertexShaderName(VSHADER);
	this->setFragmentShaderName(FSHADER);
	this->initialize(&inputAttributes, &outputAttributes);

	// Identifico los uniforms a utilizar
	this->pvmMatrixUniform = new GLSLUniform("PVMmatrix", this);
	this->imageSizeUniform = new GLSLUniform("imageSize", this);
	this->textureToBlurUniform = new GLSLUniform("textureToBlur", this);
	this->weightUniform = new GLSLUniform("weight", this);
	this->positionIncrementUniform = new GLSLUniform("positionIncrement", this);
}
