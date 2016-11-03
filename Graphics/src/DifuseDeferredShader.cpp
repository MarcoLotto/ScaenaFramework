/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "DifuseDeferredShader.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/DifuseDeferredShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/DifuseDeferredShader.frag"
#define TO_LOWERSHADER_DISTANCE 80.0f //Se define la distancia para que cambie el shader a uno mas básico
//********************************************************************************************************

void DifuseDeferredShader::init(){
	this->vertexShaderFilename = VSHADER;
	this->fragmentShaderFilename = FSHADER;

	list<string> inputAttributes;
	inputAttributes.push_back("VertexPosition");
	inputAttributes.push_back("VertexNormal");
	inputAttributes.push_back("VertexTexCoord");

	// Permito a los hijos de esta clase agregar mas atributos de entrada o salida
	this->difuseShaderChildsExtendedAttributesInit(&inputAttributes);
	
	list<string> outputAttributes;
	outputAttributes.push_back("NormalData");
	outputAttributes.push_back("ColorData");
	outputAttributes.push_back("MaterialData");

	this->setVertexShaderName(this->vertexShaderFilename);
	this->setFragmentShaderName(this->fragmentShaderFilename);
	this->initialize(&inputAttributes, &outputAttributes);

	// Permito a los hijos de esta clase agregar mas uniforms y otros componentes de inicializacion
	this->difuseShaderChildsExtendedUniformsInit();

	// Identifico los uniforms a utilizar
	this->pvmMatrixUniform = new GLSLUniform("PVMmatrix", this);
	this->normalMatrixUniform = new GLSLUniform("normalMatrix", this);
	this->diffuseMapUniform = new GLSLUniform("diffuseMap", this);
	        
     //PARAMATROS DE OPTIMIZACION************************************************
	 this->setLowerShader(NULL);  //A que shader pasa al optimizar
	 this->setDistanceToChangeShader(TO_LOWERSHADER_DISTANCE); //Distancia en la que se optimizará
	 //**************************************************************************
}