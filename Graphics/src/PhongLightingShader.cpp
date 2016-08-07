/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include <glm/glm.hpp> 
#include "GLSLProgram.h"
#include "LightingManager.h"
#include "WindowConnector.h"
#include <string>
using namespace std;

#include "PhongLightingShader.h"
#include "MeshFactory.h"
#include "VideoConfiguration.h"
#include "GraphicDevice.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/PhongLightingShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/PhongLightingShader.frag"
#define CANT_VERTICES 4
//********************************************************************************************************

PhongLightingShader* PhongLightingShader::instance = NULL;

PhongLightingShader* PhongLightingShader::getInstance(){
	if(instance==NULL){
		instance = new PhongLightingShader();
		instance->init();		
	}
	return instance;
}

PhongLightingShader::PhongLightingShader() : GLSLProgram(){	
	this->texPosition = NULL;
	this->texNormal = NULL;
	this->texDiffuse = NULL;
	this->texMaterial = NULL;

	//Creo el rectangulo de pantalla
	this->rectangleMesh = MeshFactory::getInstance()->createRectangleMesh();
}

void PhongLightingShader::prepareForDraw(Texture* position, Texture* normal, Texture* diffuse, Texture* material){
	this->texPosition = position;
	this->texNormal = normal;
	this->texDiffuse = diffuse;
	this->texMaterial = material;
}

void PhongLightingShader::draw(LightingManager *sisIlum, glm::mat4 sceneView, glm::mat4 sceneProy, glm::mat4 modelMat, glm::mat4 proyMat){
		
	//Activo el handler
	this->use();

	//Cargo matrices de modelo y proyeccion para el cuadrado sobre el que se renderiza
	this->setUniform(this->modelMatrixUniform, modelMat);
	this->setUniform(this->proyMatrixUniform, proyMat);

	// Cargo la matriz de proyeccion de la escena la cual se renderizo (para hacer lineal el depth)
	this->setUniform(this->sceneProjUniform, sceneProy);

	//Seteo las texturas del deferred shading pasada 1 a 2
	this->texPosition->assignToShader(this, this->depthTexUniform, 0);
	this->texNormal->assignToShader(this, this->normalTexUniform, 1);
	this->texDiffuse->assignToShader(this, this->colorTexUniform, 2);
	this->texMaterial->assignToShader(this, this->materialTexUniform, 3);	
	
	//Aplico la correcion gamma
	this->setUniform(this->gammaUniform, VideoConfiguration::getInstance()->getGammaCorrection());
	
	//Proceso la iluminacion	
	sisIlum->renderLights(this, this->lightUniformsContainer, sceneView, 4);
	
	//Mando a renderizar
	GraphicDevice::getInstance()->drawMesh(this->rectangleMesh);
}

void PhongLightingShader::init(){
	list<string> inputAttributes;
	inputAttributes.push_back("VertexPosition");
		
	list<string> outputAttributes;
	outputAttributes.push_back("VertexDefaultTexCoord");
	
	this->setVertexShaderName(VSHADER);
	this->setFragmentShaderName(FSHADER);
	this->initialize(&inputAttributes, &outputAttributes);

	// Identifico los uniforms a utilizar
	this->modelMatrixUniform = new GLSLUniform("modelMatrix", this);
	this->proyMatrixUniform = new GLSLUniform("proyMatrix", this);
	this->sceneProjUniform = new GLSLUniform("sceneProj", this);
	this->depthTexUniform = new GLSLUniform("DepthTex", this);
	this->normalTexUniform = new GLSLUniform("NormalTex", this);
	this->colorTexUniform = new GLSLUniform("ColorTex", this);
	this->materialTexUniform = new GLSLUniform("MaterialTex", this);
	this->gammaUniform = new GLSLUniform("gamma", this);

	// Defino un contenedor para los uniforms de iluminacion
	this->lightUniformsContainer = new LightUniformsContainer(this);
}