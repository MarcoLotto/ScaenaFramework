/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "MergeTwoImagesShader.h"
#include "MeshFactory.h"
#include "GraphicDevice.h"
#include "WindowConnector.h"
#include <vector>
#include <algorithm>
using namespace std;

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/MergeTwoImagesShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/MergeTwoImagesShader.frag"
#define CANT_VERTICES 4
//********************************************************************************************************

MergeTwoImagesShader* MergeTwoImagesShader::instance = NULL;

MergeTwoImagesShader* MergeTwoImagesShader::getInstance(){
	if(instance==NULL){
		instance = new MergeTwoImagesShader();
		instance->init();
		instance->initProyectionMatrix();
		instance->rectangleMesh = MeshFactory::getInstance()->createRectangleMesh();
	}
	return instance;
}

void MergeTwoImagesShader::initProyectionMatrix(){
	glm::vec3 escalaImagen = glm::vec3(2.0f, -2.0f, -2.0f);
	glm::mat4 proyeccionOrtogonal = glm::mat4(1.0f);
	proyeccionOrtogonal[2][2] = 0.0f;	
	glm::mat4 model = glm::scale(glm::mat4(1.0f), escalaImagen);
	this->PVMmatrix = proyeccionOrtogonal * model;
}

void MergeTwoImagesShader::draw(Texture* image1, Texture* image2, float transparency, int operation){
	//Activo el programa de shaders
	this->use();
	GraphicDevice* graphicDevice = GraphicDevice::getInstance();

	// La matriz de modelo-vista-perspectiva para el cuadrado de la pantalla
	this->setUniform(this->pvmMatrixUniform, this->PVMmatrix);

	//Cargo las dos imagenes a mergear, la transparencia de la segunda textura y la operacion con la que se mergearan
	image1->assignToShader(this, this->image1Uniform, 0);
	image2->assignToShader(this, this->image2Uniform, 1);
	this->setUniform(this->transparencyUniform, transparency);
	this->setUniform(this->operationUniform, operation);

	//Mando a renderizar
	graphicDevice->drawMesh(this->rectangleMesh);
}

void MergeTwoImagesShader::init(){		
	list<string> inputAttributes;
	inputAttributes.push_back("VertexPosition");
	inputAttributes.push_back("TextureCoords");
	
	list<string> outputAttributes;
	outputAttributes.push_back("textureData");

	this->setVertexShaderName(VSHADER);
	this->setFragmentShaderName(FSHADER);
	this->initialize(&inputAttributes, &outputAttributes);

	// Identifico los uniforms a utilizar
	this->pvmMatrixUniform = new GLSLUniform("PVMmatrix", this);
	this->transparencyUniform = new GLSLUniform("transparency", this);
	this->image1Uniform = new GLSLUniform("image1", this);
	this->image2Uniform = new GLSLUniform("image2", this);
	this->operationUniform = new GLSLUniform("operation", this);
}
