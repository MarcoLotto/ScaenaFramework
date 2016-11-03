/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ImageColorChangerShader.h"
#include "MeshFactory.h"
#include "GraphicDevice.h"
#include "WindowConnector.h"
#include "StringUtils.h"
#include <vector>
using namespace std;

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/ImageColorChangerShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/ImageColorChangerShader.frag"
#define CANT_VERTICES 4
#define MAX_COLORS_TO_CHANGE 10  // Si se quiere cambiar mas de 10 colores en una imagen cambiar este parametro (Y EL SHADER!)
//********************************************************************************************************

ImageColorChangerShader* ImageColorChangerShader::instance = NULL;

ImageColorChangerShader* ImageColorChangerShader::getInstance(){
	if(instance==NULL){
		instance = new ImageColorChangerShader();
		instance->init();
		instance->initProyectionMatrix();
		instance->rectangleMesh = MeshFactory::getInstance()->createRectangleMesh();
	}
	return instance;
}

void ImageColorChangerShader::initProyectionMatrix(){
	glm::vec3 escalaImagen = glm::vec3(2.0f, -2.0f, -2.0f);
	glm::mat4 proyeccionOrtogonal = glm::mat4(1.0f);
	proyeccionOrtogonal[2][2] = 0.0f;	
	glm::mat4 model = glm::scale(glm::mat4(1.0f), escalaImagen);
	this->PVMmatrix = proyeccionOrtogonal * model;
}

void ImageColorChangerShader::draw(Texture* image, vector<vec3>* rgbInputColors, vector<vec3>* rgbOutputColors, float colorTolerance){
	//Activo el programa de shaders
	this->use();
	GraphicDevice* graphicDevice = GraphicDevice::getInstance();

	// La matriz de modelo-vista-perspectiva para el cuadrado de la pantalla
	this->setUniform(this->pvmMatrixUniform, this->PVMmatrix);

	// Evito que difieran la cantidad de colores de entrada y salida
	int colorsCount = rgbInputColors->size();
	if(rgbOutputColors->size() < colorsCount){
		colorsCount = rgbOutputColors->size();
	}

	// Asignamos los colores al shader (los colores a ser reemplazados en la imagen y por cuales se reemplazaran)	
	for(unsigned int i=0; i < colorsCount; i++){
		this->setUniform(this->inputColorsUniform[i], rgbInputColors->at(i));
		this->setUniform(this->outputColorsUniform[i], rgbOutputColors->at(i));
	}

	// Asignamos la imagen y la cantidad de colores a cambiar
	image->assignToShader(this, this->imageUniform, 0);	
	this->setUniform(this->colorsCountUniform, colorsCount);
	this->setUniform(this->colorToleranceUniform, colorTolerance);

	//Mando a renderizar
	graphicDevice->drawMesh(this->rectangleMesh);
}

void ImageColorChangerShader::init(){		
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
	this->imageUniform = new GLSLUniform("image", this);
	this->colorToleranceUniform = new GLSLUniform("colorTolerance", this);
	this->colorsCountUniform = new GLSLUniform("colorsCount", this);
	for(unsigned int i=0; i < MAX_COLORS_TO_CHANGE; i++){
		string arrayIndex = StringUtils::toString(i);
		this->inputColorsUniform.push_back( new GLSLUniform(("inputColors[" + arrayIndex + "]").c_str(), this) );
		this->outputColorsUniform.push_back( new GLSLUniform(("outputColors[" + arrayIndex + "]").c_str(), this) );
	}
}
