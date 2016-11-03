/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "BasicForceMeshDeformationTFShader.h"
#include "GraphicDevice.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/BasicForceMeshDeformationTFShader.vert"
//********************************************************************************************************

BasicForceMeshDeformationTFShader::BasicForceMeshDeformationTFShader(){
	this->forceDirection = vec3(0.0f);
	this->forceIntensity = 0.0f;
	this->staticStrength = 0.0f;
	this->staticPoint = vec3(0.0f);
	this->forceIntensityInterpolator = new TimeParameterInterpolator(0.0f, this->forceIntensity, 0);
	this->init();
}

BasicForceMeshDeformationTFShader::~BasicForceMeshDeformationTFShader(){
	delete this->forceIntensityInterpolator;
}

// Se declaran que VBOs del MeshBuffers se escribiran en el output del transform feedback (se declaran en el transformFeedbackObject)
void BasicForceMeshDeformationTFShader::declareFeedbackInputsAndOutputs(MeshBuffers* meshBuffersInput, MeshBuffers* meshBuffersOutput){
	// Me guardo el mesh para despues poder transformar
	this->inputMeshBuffers = meshBuffersInput;

	// Inicializo (o limpio) el shader para este mesh
	this->prepareTransformFeedbackObject();

	//Declaro todos los buffers de salida (solo puse uno, podria haber mas)
	this->attachVboToTransformFeedback(meshBuffersOutput->getVertexBufferObject());
}

// Se llama para generar la transformacion. Es analogo a draw en los shaders con rasterizador
void BasicForceMeshDeformationTFShader::execute(){
	//Activo el programa de shaders
	this->use();
	
	//Cargo la direccion e intensidad de la fuerza aplicada
	this->setUniform(this->staticPointUniform, this->staticPoint);
	this->setUniform(this->staticStrengthUniform, this->staticStrength);
	this->setUniform(this->forceDirectionUniform, this->forceDirection);
	this->setUniform(this->forceIntensityUniform, this->forceIntensityInterpolator->getCurrentValue());

	//Mando a transformar
	VertexArrayObject* vao = this->inputMeshBuffers->getVaoObject();
	unsigned int vertexCount = this->inputMeshBuffers->getVertexBufferObject()->getBufferDataLenght();
	GraphicDevice::getInstance()->transformUsingFeedback(vao, vertexCount, this->transformFeedbackObject);
}

// Inicia el shader de transformacion
void BasicForceMeshDeformationTFShader::init(){
	list<string> inputAttributes;
	inputAttributes.push_back("VertexPosition");	
	
	list<string> outputAttributes;
	outputAttributes.push_back("VertexPositionOutput");

	this->setVertexShaderName(VSHADER);	
	this->initialize(&inputAttributes, &outputAttributes, true);
}


void BasicForceMeshDeformationTFShader::setAnimationTime(unsigned int time){ 
	this->forceIntensityInterpolator->setInterpolationTime(time); 
	this->forceIntensityInterpolator->setPlay();

	// Identifico los uniforms a utilizar
	this->staticPointUniform = new GLSLUniform("staticPoint", this);
	this->staticStrengthUniform = new GLSLUniform("staticStrength", this);
	this->forceDirectionUniform = new GLSLUniform("forceDirection", this);
	this->forceIntensityUniform = new GLSLUniform("forceIntensity", this);
}