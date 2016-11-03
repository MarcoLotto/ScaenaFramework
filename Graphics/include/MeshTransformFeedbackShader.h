/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "TransformFeedbackShader.h"
#include "MeshBuffers.h"

// Interfaz comun para aquellos shaders de transform feedback que deforman o transforman meshes
class MeshTransformFeedbackShader : public TransformFeedbackShader{

protected:
	MeshBuffers* inputMeshBuffers;

public:
	MeshTransformFeedbackShader() : TransformFeedbackShader(){};	

	// Se declaran que VBOs del MeshBuffers se escribiran en el output del transform feedback (se declaran en el transformFeedbackObject)
	virtual void declareFeedbackInputsAndOutputs(MeshBuffers* meshBuffersInput, MeshBuffers* meshBuffersOutput) = 0;

	// Se llama para generar la transformacion. Es analogo a draw en los shaders con rasterizador
	virtual void execute() = 0;	
};