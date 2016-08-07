/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshTransformFeedbackShader.h"
#include "TimeParameterInterpolator.h"
#include "VertexBufferObject.h"
#include "VertexArrayObject.h"

class MeshKeyframeAnimationTF : public MeshTransformFeedbackShader{

private:
	VertexArrayObject* vao;
	TimeParameterInterpolator* animationInterpolator;

	void loadMeshBuffersInVao(MeshBuffers* meshBuffers);

	GLSLUniform* animationPointUniform;

public:
	MeshKeyframeAnimationTF(MeshBuffers* destinyMeshBuffers);
	virtual ~MeshKeyframeAnimationTF();

	// Se declaran que VBOs del MeshBuffers se escribiran en el output del transform feedback (se declaran en el transformFeedbackObject)
	void declareFeedbackInputsAndOutputs(MeshBuffers* meshBuffersInput, MeshBuffers* meshBuffersOutput);

	// Se llama para generar la transformacion. Es analogo a draw en los shaders con rasterizador
	void execute();	

	// Inicia el shader de transformacion
	void init();
	
	// Si se desea animacion se debe setear el tiempo que esta durara
	void setAnimationTime(unsigned int time);

	// Setea si al hacer loop la animacion debe volver al principio o invertirse el sentido
	void isAnimationReverseLoopEnabled(bool enabled){ this->animationInterpolator->setInverseAnimationEnabled(enabled); }
	
	// Para prender o apagar la animacion
	void playAnimation(){ this->animationInterpolator->setPlay(); }
	void stopAnimation(){ this->animationInterpolator->setStop(); }
};