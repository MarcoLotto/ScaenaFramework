/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "TransformFeedbackShader.h"
#include "InvalidVBOBufferPassedToTransformFeedbackException.h"

TransformFeedbackShader::TransformFeedbackShader() : GLSLProgram() { 
	this->transformFeedbackObject = NULL; 
}

TransformFeedbackShader::~TransformFeedbackShader(){
	if(this->transformFeedbackObject != NULL){
		delete this->transformFeedbackObject;
	}
}

// Utilizar antes de empezar una declaracion de outputs. Crea o renueva el transformFeedbackObject
void TransformFeedbackShader::prepareTransformFeedbackObject(){
	if(this->transformFeedbackObject != NULL){
		delete this->transformFeedbackObject;
	}
	this->transformFeedbackObject = new TransformFeedbackObject();
}

// Si el vbo no es nulo, lo attachea al transformFeedBackObject (si este no existe, lo crea)
void TransformFeedbackShader::attachVboToTransformFeedback(VertexBufferObject* vbo){
	if(vbo == NULL){
		throw new InvalidVBOBufferPassedToTransformFeedbackException();
	}
	this->transformFeedbackObject->attachVertexBufferObject(vbo);
}

