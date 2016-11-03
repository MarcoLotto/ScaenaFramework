/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "BloomShader.h"
#include "MeshFactory.h"
#include "VideoConfiguration.h"
#include "GraphicDevice.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/BloomShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/BloomShader.frag"
#define CANT_VERTICES 4
#define ITERATIONS 3
//********************************************************************************************************

BloomShader* BloomShader::instance = NULL;

BloomShader* BloomShader::getInstance(){
	if(instance==NULL){
		instance = new BloomShader();
		instance->init();
		instance->initProyectionMatrix();
		instance->rectangleMesh = MeshFactory::getInstance()->createRectangleMesh();
	}
	return instance;
}

void BloomShader::initProyectionMatrix(){
	glm::vec3 escalaImagen = glm::vec3(2.0f, -2.0f, -2.0f);
	glm::mat4 proyeccionOrtogonal = glm::mat4(1.0f);
	proyeccionOrtogonal[2][2] = 0.0f;	
	glm::mat4 model = glm::scale(glm::mat4(1.0f), escalaImagen);
	this->PVMmatrix = proyeccionOrtogonal * model;
}

void BloomShader::drawFirstPass(Texture* original, vec2 imageSize, float* weights, float* posIncrement, float lumThreshold){

	//Activo el programa de shaders
	this->use();
	GraphicDevice* graphicDevice = GraphicDevice::getInstance();

	//Cargo matrices de modelo, view, proyeccion y normal
	this->setUniform(this->pvmMatrixUniform, this->PVMmatrix);
	this->setUniform(this->imageSizeUniform, imageSize);
	original->assignToShader(this, this->textureToBlurUniform, 0);	
	this->setUniform(this->lumThresholdUniform, lumThreshold);
		
	this->setVectorUniform(this->weightUniform, ITERATIONS, weights);
	this->setVectorUniform(this->positionIncrementUniform, ITERATIONS, posIncrement);

	//Activo la primer subrutina
	this->changeSubroutineInFragmentShader("pass1");

	//Mando a renderizar la primer pasada
	graphicDevice->drawMesh(this->rectangleMesh);
}

void BloomShader::drawSecondPass(Texture* auxiliary, vec2 imageSize, float* weights, float* posIncrement, float lumThreshold){

	//Activo el programa de shaders
	this->use();
	GraphicDevice* graphicDevice = GraphicDevice::getInstance();

	//Cargo matrices de modelo, view, proyeccion y normal
	this->setUniform(this->pvmMatrixUniform, this->PVMmatrix);
	this->setUniform(this->imageSizeUniform, imageSize);
	auxiliary->assignToShader(this, this->textureToBlurUniform, 0);	
	this->setUniform(this->lumThresholdUniform, lumThreshold);
		
	this->setVectorUniform(this->weightUniform, ITERATIONS, weights);
	this->setVectorUniform(this->positionIncrementUniform, ITERATIONS, posIncrement);

	//Activo la segunda subrutina
	this->changeSubroutineInFragmentShader("pass2");

	//Mando a renderizar la segunda pasada
	graphicDevice->drawMesh(this->rectangleMesh);
}

void BloomShader::drawThirdPass(Texture* original, Texture* auxiliary, vec2 imageSize, float* weights, float* posIncrement, float lumThreshold){

	//Activo el programa de shaders
	this->use();
	GraphicDevice* graphicDevice = GraphicDevice::getInstance();

	//Cargo matrices de modelo, view, proyeccion y normal
	this->setUniform(this->pvmMatrixUniform, this->PVMmatrix);
	this->setUniform(this->imageSizeUniform, imageSize);
	auxiliary->assignToShader(this, this->textureToBlurUniform, 0);	
	this->setUniform(this->lumThresholdUniform, lumThreshold);
		
	this->setVectorUniform(this->weightUniform, ITERATIONS, weights);
	this->setVectorUniform(this->positionIncrementUniform, ITERATIONS, posIncrement);
	
	//Activo la tercera subrutina
	this->changeSubroutineInFragmentShader("pass3");

	//Le seteo a la tercera pasada la imagen original tambien
	original->assignToShader(this, this->originalTextureUniform, 1);		

	//Mando a renderizar la tercera pasada
	graphicDevice->drawMesh(this->rectangleMesh);
}

void BloomShader::init(){

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
	this->lumThresholdUniform = new GLSLUniform("lumThreshold", this);	
	this->weightUniform = new GLSLUniform("weight", this);
	this->positionIncrementUniform = new GLSLUniform("positionIncrement", this);
	this->originalTextureUniform = new GLSLUniform("originalTexture", this);	
}
