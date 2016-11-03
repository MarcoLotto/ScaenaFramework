/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UIShader.h"
#include "MeshFactory.h"
#include "VideoConfiguration.h"
#include "GraphicDevice.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/UIShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/UIShader.frag"
#define CANT_VERTICES 4
#define ITERATIONS 3
//********************************************************************************************************

UIShader* UIShader::instance = NULL;

UIShader* UIShader::getInstance(){
	if(instance==NULL){
		instance = new UIShader();
		instance->init();
		instance->initProyectionMatrix();
		instance->rectangleMesh = MeshFactory::getInstance()->createUIRectangleMesh();
		instance->sideCenteredRectangleMesh = MeshFactory::getInstance()->createRectangleMesh();
	}
	return instance;
}

void UIShader::initProyectionMatrix(){
	glm::vec3 escalaImagen = glm::vec3(2.0f, -2.0f, -2.0f);
	glm::mat4 proyeccionOrtogonal = glm::mat4(1.0f);
	proyeccionOrtogonal[2][2] = 0.0f;	
	glm::mat4 model = glm::scale(glm::mat4(1.0f), escalaImagen);
	this->PVmatrix = proyeccionOrtogonal * model;
}

void UIShader::draw(Texture* textureToShow, vec2 partialSize, vec2 positionIndex, mat4 modelMatrix, float transparency){

	//Activo el programa de shaders
	this->use();

	//Cargo matrices de modelo, view, proyeccion y normal
	this->setUniform(this->pvmMatrixUniform, this->PVmatrix * modelMatrix);
	this->setUniform(this->partialSizeUniform, partialSize);
	this->setUniform(this->positionIndexUniform, positionIndex);
	this->setUniform(this->transparencyUniform, transparency);
	textureToShow->assignToShader(this, this->textureDataUniform, 0);
			
	//Mando a renderizar la segunda pasada
	GraphicDevice::getInstance()->drawMesh(this->rectangleMesh);
}

void UIShader::drawSideCenteredCuad(Texture* textureToShow, vec2 partialSize, vec2 positionIndex, mat4 modelMatrix, float transparency){

	//Activo el programa de shaders
	this->use();

	//Cargo matrices de modelo, view, proyeccion y normal
	this->setUniform(this->pvmMatrixUniform, this->PVmatrix * modelMatrix);
	this->setUniform(this->partialSizeUniform, partialSize);
	this->setUniform(this->positionIndexUniform, positionIndex);
	this->setUniform(this->transparencyUniform, transparency);
	textureToShow->assignToShader(this, this->textureDataUniform, 0);
			
	//Mando a renderizar la segunda pasada
	GraphicDevice::getInstance()->drawMesh(this->sideCenteredRectangleMesh);
}

void UIShader::init(){
		
	list<string> inputAttributes;
	inputAttributes.push_back("VertexPosition");
	inputAttributes.push_back("TextureCoords");
		
	list<string> outputAttributes;
	this->setVertexShaderName(VSHADER);
	this->setFragmentShaderName(FSHADER);
	this->initialize(&inputAttributes, &outputAttributes);	

	// Identifico los uniforms a utilizar
	this->pvmMatrixUniform = new GLSLUniform("PVMmatrix", this);
	this->partialSizeUniform = new GLSLUniform("partialSize", this);
	this->positionIndexUniform = new GLSLUniform("positionIndex", this);
	this->transparencyUniform = new GLSLUniform("transparency", this);	
	this->textureDataUniform = new GLSLUniform("textureData", this);
}
