/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "RandomTextureGeneratorShader.h"
#include "MeshFactory.h"
#include "GraphicDevice.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/TextureGeneratorShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/TextureGeneratorShader.frag"
#define CANT_VERTICES 4
//********************************************************************************************************

RandomTextureGeneratorShader* RandomTextureGeneratorShader::instance = NULL;

RandomTextureGeneratorShader* RandomTextureGeneratorShader::getInstance(){
	if(instance==NULL){
		instance = new RandomTextureGeneratorShader();
		instance->init();
		instance->initProyectionMatrix();
		instance->rectangleMesh = MeshFactory::getInstance()->createRectangleMesh();
	}
	return instance;
}

void RandomTextureGeneratorShader::initProyectionMatrix(){
	glm::vec3 escalaImagen = glm::vec3(2.0f, -2.0f, -2.0f);
	glm::mat4 proyeccionOrtogonal = glm::mat4(1.0f);
	proyeccionOrtogonal[2][2] = 0.0f;	
	glm::mat4 model = glm::scale(glm::mat4(1.0f), escalaImagen);
	this->PVMmatrix = proyeccionOrtogonal * model;
}

void RandomTextureGeneratorShader::draw(vec2 sizeInPixels, float minRange, float maxRange){

	//Activo el programa de shaders
	this->use();

	// Genero el vector de valores random para pasarle al shader y los cargo
	float* randomVector = this->generateRandomVector(sizeInPixels, minRange, maxRange);
	this->setVectorUniform(this->valueVectorRedUniform, sizeInPixels.x * sizeInPixels.y, randomVector);
	delete randomVector;
	randomVector = this->generateRandomVector(sizeInPixels, minRange, maxRange);
	this->setVectorUniform(this->valueVectorGreenUniform, sizeInPixels.x * sizeInPixels.y, randomVector);
	delete randomVector;
	randomVector = this->generateRandomVector(sizeInPixels, minRange, maxRange);
	this->setVectorUniform(this->valueVectorBlueUniform, sizeInPixels.x * sizeInPixels.y, randomVector);
	delete randomVector;

	//Cargo matrices de modelo-view-proyeccion
	this->setUniform(this->pvmMatrixUniform, this->PVMmatrix);
	// Cargo el tamaño en pixels de la textura
	this->setUniform(this->sizeUniform, sizeInPixels);
	
	//Mando a renderizar la primer pasada
	GraphicDevice::getInstance()->drawMesh(this->rectangleMesh);
}

// Consigue un valor random entre los dos pasados como parametro
float RandomTextureGeneratorShader::random(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

// Genero un vector con valores random
float* RandomTextureGeneratorShader::generateRandomVector(vec2 sizeInPixels, float minRange, float maxRange){
	unsigned int pixelsCount = (unsigned int) sizeInPixels.x * sizeInPixels.y;
	float* randomVector = new float[pixelsCount]();
	for (int i = 0; i < pixelsCount; i++) {
		randomVector[i] = random(minRange, maxRange);
	}
	return randomVector;
}

void RandomTextureGeneratorShader::init(){		
	list<string> inputAttributes;
	inputAttributes.push_back("VertexPosition");
	inputAttributes.push_back("TextureCoords");
	
	list<string> outputAttributes;
	outputAttributes.push_back("textureData");

	this->setVertexShaderName(VSHADER);
	this->setFragmentShaderName(FSHADER);
	this->initialize(&inputAttributes, &outputAttributes);

	// Identifico los uniforms a utilizar
	this->valueVectorRedUniform = new GLSLUniform("valueVectorRed", this);
	this->valueVectorGreenUniform = new GLSLUniform("valueVectorGreen", this);
	this->valueVectorBlueUniform = new GLSLUniform("valueVectorBlue", this);
	this->pvmMatrixUniform = new GLSLUniform("PVMmatrix", this);
	this->sizeUniform = new GLSLUniform("size", this);
	
}
