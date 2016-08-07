/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "MeshKeyframeAnimationTF.h"
#include "GraphicDevice.h"
#include "WindowConnector.h"

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/MeshKeyframeAnimationTF.vert"
//********************************************************************************************************

MeshKeyframeAnimationTF::MeshKeyframeAnimationTF(MeshBuffers* destinyMeshBuffers){
	this->vao = new VertexArrayObject();
	this->loadMeshBuffersInVao(destinyMeshBuffers);
	this->animationInterpolator = new TimeParameterInterpolator(0.0f, 1.0f, 0);
	this->init();
}

MeshKeyframeAnimationTF::~MeshKeyframeAnimationTF(){
	delete this->animationInterpolator;
	delete this->vao;
}

// Se declaran que VBOs del MeshBuffers se escribiran en el output del transform feedback (se declaran en el transformFeedbackObject)
void MeshKeyframeAnimationTF::declareFeedbackInputsAndOutputs(MeshBuffers* meshBuffersInput, MeshBuffers* meshBuffersOutput){
	// Me guardo el mesh para despues poder transformar
	this->inputMeshBuffers = meshBuffersInput;

	// Me guardo los buffers del mesh en el vao del TF
	this->loadMeshBuffersInVao(meshBuffersInput);

	// Inicializo (o limpio) el shader para este mesh
	this->prepareTransformFeedbackObject();

	//Declaro todos los buffers de salida (solo puse uno, podria haber mas)
	this->attachVboToTransformFeedback(meshBuffersOutput->getVertexBufferObject());
	this->attachVboToTransformFeedback(meshBuffersOutput->getNormalBufferObject());
	this->attachVboToTransformFeedback(meshBuffersOutput->getTangentBufferObject());
	this->attachVboToTransformFeedback(meshBuffersOutput->getBinormalBufferObject());
}

// Se llama para generar la transformacion. Es analogo a draw en los shaders con rasterizador
void MeshKeyframeAnimationTF::execute(){
	//Activo el programa de shaders
	this->use();
	
	//Cargo en donde estoy en la animacion (entre 0 y 1)
	this->setUniform(this->animationPointUniform, this->animationInterpolator->getCurrentValue());

	//Mando a transformar	
	unsigned int vertexCount = this->inputMeshBuffers->getVertexBufferObject()->getBufferDataLenght();
	GraphicDevice::getInstance()->transformUsingFeedback(this->vao, vertexCount, this->transformFeedbackObject);
}

// Inicia el shader de transformacion
void MeshKeyframeAnimationTF::init(){
	list<string> inputAttributes;
	inputAttributes.push_back("EndVertexPosition");	
	inputAttributes.push_back("EndNormalPosition");	
	inputAttributes.push_back("EndTangentPosition");	
	inputAttributes.push_back("EndBinormalPosition");	

	inputAttributes.push_back("StartVertexPosition");
	inputAttributes.push_back("StartNormalPosition");	
	inputAttributes.push_back("StartTangentPosition");	
	inputAttributes.push_back("StartBinormalPosition");
	
	list<string> outputAttributes;
	outputAttributes.push_back("VertexPositionOutput");
	outputAttributes.push_back("NormalPositionOutput");
	outputAttributes.push_back("TangentPositionOutput");
	outputAttributes.push_back("BinormalPositionOutput");

	this->setVertexShaderName(VSHADER);	
	this->initialize(&inputAttributes, &outputAttributes, true);

	// Identifico los uniforms a utilizar
	this->animationPointUniform = new GLSLUniform("animationPoint", this);
}


void MeshKeyframeAnimationTF::setAnimationTime(unsigned int time){ 
	this->animationInterpolator->setInterpolationTime(time); 
	this->animationInterpolator->setPlay();
}

void MeshKeyframeAnimationTF::loadMeshBuffersInVao(MeshBuffers* meshBuffers){	
	if(meshBuffers->getVertexBufferObject() != NULL)
		this->vao->addVboAttachement(meshBuffers->getVertexBufferObject());
	if(meshBuffers->getNormalBufferObject() != NULL)
		this->vao->addVboAttachement(meshBuffers->getNormalBufferObject());
	if(meshBuffers->getTangentBufferObject() != NULL)
		this->vao->addVboAttachement(meshBuffers->getTangentBufferObject());
	if(meshBuffers->getBinormalBufferObject() != NULL)
		this->vao->addVboAttachement(meshBuffers->getBinormalBufferObject());
}