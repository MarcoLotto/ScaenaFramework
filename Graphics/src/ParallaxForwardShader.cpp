/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ParallaxForwardShader.h"
#include "ShaderManager.h"
#include "VideoConfiguration.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/ParallaxForwardShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/ParallaxForwardShader.frag"
//********************************************************************************************************

ParallaxForwardShader::ParallaxForwardShader(){
	this->initOptimizationsParameters();
}

void ParallaxForwardShader::init(){
	this->vertexShaderFilename = VSHADER;
	this->fragmentShaderFilename = FSHADER;

	list<string> inputAttributes;
	inputAttributes.push_back("VertexPosition");
	inputAttributes.push_back("VertexNormal");
	inputAttributes.push_back("VertexTexCoord");
	inputAttributes.push_back("VertexTangent");
	inputAttributes.push_back("VertexBinormal");

	// Permito a los hijos de esta clase agregar mas atributos de entrada o salida
	this->parallaxShaderChildsExtendedAttributesInit(&inputAttributes);
	
	list<string> outputAttributes;
	outputAttributes.push_back("FragmentData");

	this->setVertexShaderName(this->vertexShaderFilename);
	this->setFragmentShaderName(this->fragmentShaderFilename);
	this->initialize(&inputAttributes, &outputAttributes); 

	// Permito a los hijos de esta clase agregar mas uniforms y otros componentes de inicializacion
	this->parallaxShaderChildsExtendedUniformsInit();

	// Identifico los uniforms a utilizar
	this->pvmMatrixUniform = new GLSLUniform("PVMmatrix", this);
	this->normalMatrixUniform = new GLSLUniform("normalMatrix", this);
	this->atenuationFactorUniform = new GLSLUniform("atenuationFactor", this);
	this->vmMatrixUniform = new GLSLUniform("VMmatrix", this);
	this->gammaUniform = new GLSLUniform("gamma", this);

	// Defino un contenedor para los uniforms de iluminacion
	this->lightUniformsContainer = new LightUniformsContainer(this);
}

void ParallaxForwardShader::extendedDraw(Mesh* mesh){
	// Los hijos cargan su data de draw
	this->parallaxShaderChildsExtendedDraw(mesh);

	// Cargo en shader la informacion de las luces
	ShaderManager::getInstance()->getCurrentScene()->getLightingManager()->renderLights(this, this->lightUniformsContainer, mesh->getDrawPacket()->getViewMatrix(), 3);

	//Aplico la correcion gamma
	this->setUniform(this->gammaUniform, VideoConfiguration::getInstance()->getGammaCorrection());
}