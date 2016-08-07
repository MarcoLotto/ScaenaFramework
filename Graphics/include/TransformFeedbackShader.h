/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GLSLProgram.h"
#include "VertexArrayObject.h"
#include "TransformFeedbackObject.h"

// Interfaz comun para aquellos shaders de transform feedback
class TransformFeedbackShader : public GLSLProgram{

protected:
	// En el se definen que VBOs seran a los que vayaq la salida de la transformacion
	TransformFeedbackObject* transformFeedbackObject;

	// Utilizar antes de empezar una declaracion de outputs. Crea o renueva el transformFeedbackObject
	void prepareTransformFeedbackObject();

	// Si el vbo no es nulo, lo attachea al transformFeedBackObject (si este no existe, lo crea)
	void attachVboToTransformFeedback(VertexBufferObject* vbo);

public:
	TransformFeedbackShader();
	virtual ~TransformFeedbackShader();

	// Se llama para generar la transformacion. Es analogo a draw en los shaders con rasterizador
	virtual void execute() = 0;

	// Devuelve el feedback object para poder dibujar
	TransformFeedbackObject* getTransformFeedbackObject(){ return this->transformFeedbackObject; }
};