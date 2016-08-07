/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "DepthOfFieldShader.h"
#include "MeshFactory.h"
#include "VideoConfiguration.h"
#include "GraphicDevice.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/DepthOfFieldShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/DepthOfFieldShader.frag"
#define CANT_VERTICES 4
#define ITERATIONS 3
//********************************************************************************************************

DepthOfFieldShader* DepthOfFieldShader::instance = NULL;

DepthOfFieldShader* DepthOfFieldShader::getInstance(){
	if(instance==NULL){
		instance = new DepthOfFieldShader();
		instance->init();
		instance->initProyectionMatrix();
		instance->rectangleMesh = MeshFactory::getInstance()->createRectangleMesh();
	}
	return instance;
}

void DepthOfFieldShader::initProyectionMatrix(){
	glm::vec3 escalaImagen = glm::vec3(2.0f, -2.0f, -2.0f);
	glm::mat4 proyeccionOrtogonal = glm::mat4(1.0f);
	proyeccionOrtogonal[2][2] = 0.0f;	
	glm::mat4 model = glm::scale(glm::mat4(1.0f), escalaImagen);
	this->PVMmatrix = proyeccionOrtogonal * model;
}

void DepthOfFieldShader::drawFirstPass(Texture* textureToBlur, Texture* depthTexture, vec2 imageSize, float* weights, float* posIncrement, 
										float depthAtBlurStart, bool blurToFront, mat4 sceneProj, float blurFalloff){
	//Activo el programa de shaders
	this->use();
	GraphicDevice* graphicDevice = GraphicDevice::getInstance();

	//Cargo matrices de modelo, view, proyeccion y normal
	this->setUniform(this->pvmMatrixUniform, this->PVMmatrix);
	this->setUniform(this->imageSizeUniform, imageSize);
	textureToBlur->assignToShader(this, this->textureToBlurUniform, 0);
	depthTexture->assignToShader(this, this->depthTextureUniform, 1);

	// Utilizado para pasar a espacio lineal el depth
	this->setUniform(this->sceneProjUniform, sceneProj);

	//Seteo desde que depth empieza a aplicarse blur y si el blur se aplica hacia atras o hacia adelante de la pantalla
	this->setUniform(this->depthAtBlurStartUniform, depthAtBlurStart);
	this->setUniform(this->blurToFrontUniform, (int) blurToFront);

	// El factor de decaimiento del blur (es decir que tan rapido llegara el blur a su maximo una vez alcanzado depthAtBlurStart) 
	this->setUniform(this->blurFalloffUniform, blurFalloff);
				
	this->setVectorUniform(this->weightUniform, ITERATIONS, weights);
	this->setVectorUniform(this->positionIncrementUniform, ITERATIONS, posIncrement);

	//Activo la primer subrutina
	this->changeSubroutineInFragmentShader("pass1");

	//Mando a renderizar la primer pasada
	graphicDevice->drawMesh(this->rectangleMesh);
}

void DepthOfFieldShader::drawSecondPass(Texture* textureToBlur, Texture* depthTexture, vec2 imageSize, float* weights, float* posIncrement, 
										float depthAtBlurStart, bool blurToFront, mat4 sceneProj, float blurFalloff){
	//Activo el programa de shaders
	this->use();
	GraphicDevice* graphicDevice = GraphicDevice::getInstance();

	//Cargo matrices de modelo, view, proyeccion y normal
	this->setUniform(this->pvmMatrixUniform, this->PVMmatrix);
	this->setUniform(this->imageSizeUniform, imageSize);
	textureToBlur->assignToShader(this, this->textureToBlurUniform, 0);
	depthTexture->assignToShader(this, this->depthTextureUniform, 1);

	// Utilizado para pasar a espacio lineal el depth
	this->setUniform(this->sceneProjUniform, sceneProj);

	//Seteo desde que depth empieza a aplicarse blur y si el blur se aplica hacia atras o hacia adelante de la pantalla
	this->setUniform(this->depthAtBlurStartUniform, depthAtBlurStart);
	this->setUniform(this->blurToFrontUniform, (unsigned int) blurToFront);

	// El factor de decaimiento del blur (es decir que tan rapido llegara el blur a su maximo una vez alcanzado depthAtBlurStart) 
	this->setUniform(this->blurFalloffUniform, blurFalloff);
			
	this->setVectorUniform(this->weightUniform, ITERATIONS, weights);
	this->setVectorUniform(this->positionIncrementUniform, ITERATIONS, posIncrement);

	//Activo la segunda subrutina
	this->changeSubroutineInFragmentShader("pass2");

	//Mando a renderizar la segunda pasada
	graphicDevice->drawMesh(this->rectangleMesh);
}

void DepthOfFieldShader::init(){
		
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
	this->depthTextureUniform = new GLSLUniform("depthTexture", this);
	this->sceneProjUniform = new GLSLUniform("sceneProj", this);
	this->depthAtBlurStartUniform = new GLSLUniform("depthAtBlurStart", this);
	this->blurToFrontUniform = new GLSLUniform("blurToFront", this);
	this->blurFalloffUniform = new GLSLUniform("blurFalloff", this);
	this->weightUniform = new GLSLUniform("weight", this);
	this->positionIncrementUniform = new GLSLUniform("positionIncrement", this);
}
