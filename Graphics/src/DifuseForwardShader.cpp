/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "DifuseForwardShader.h"
#include "ShaderManager.h"
#include "VideoConfiguration.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/DifuseForwardShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/DifuseForwardShader.frag"
#define TO_LOWERSHADER_DISTANCE 80.0f //Se define la distancia para que cambie el shader a uno mas básico
//********************************************************************************************************

void DifuseForwardShader::init(){
	this->vertexShaderFilename = VSHADER;
	this->fragmentShaderFilename = FSHADER;

	list<string> inputAttributes;
	inputAttributes.push_back("VertexPosition");
	inputAttributes.push_back("VertexNormal");
	inputAttributes.push_back("VertexTexCoord");  

	// Permito a los hijos de esta clase agregar mas atributos de entrada o salida
	this->difuseShaderChildsExtendedAttributesInit(&inputAttributes);
	
	list<string> outputAttributes;
	outputAttributes.push_back("FragmentData");

	this->setVertexShaderName(this->vertexShaderFilename);
	this->setFragmentShaderName(this->fragmentShaderFilename);
	this->initialize(&inputAttributes, &outputAttributes);

	// Permito a los hijos de esta clase agregar mas uniforms y otros componentes de inicializacion
	this->difuseShaderChildsExtendedUniformsInit();

	// Identifico los uniforms a utilizar
	this->pvmMatrixUniform = new GLSLUniform("PVMmatrix", this);
	this->normalMatrixUniform = new GLSLUniform("normalMatrix", this);
	this->vmMatrixUniform = new GLSLUniform("VMmatrix", this);
	this->gammaUniform = new GLSLUniform("gamma", this);

	// Defino un contenedor para los uniforms de iluminacion
	this->lightUniformsContainer = new LightUniformsContainer(this);
        
     //PARAMATROS DE OPTIMIZACION************************************************
	 this->setLowerShader(NULL);  //A que shader pasa al optimizar
	 this->setDistanceToChangeShader(TO_LOWERSHADER_DISTANCE); //Distancia en la que se optimizará
	 //**************************************************************************
}

void DifuseForwardShader::extendedDraw(Mesh* mesh){
	// Los hijos cargan su data de draw
	this->difuseShaderChildsExtendedDraw(mesh);

	this->setUniform(this->vmMatrixUniform, this->vmMatrix);
	
	// Cargo en shader la informacion de las luces
	ShaderManager::getInstance()->getCurrentScene()->getLightingManager()->renderLights(this, this->lightUniformsContainer, mesh->getDrawPacket()->getViewMatrix(), 1);
	
	//Aplico la correcion gamma
	this->setUniform(this->gammaUniform, VideoConfiguration::getInstance()->getGammaCorrection());
}
