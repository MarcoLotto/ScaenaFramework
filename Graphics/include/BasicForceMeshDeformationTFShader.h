/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshTransformFeedbackShader.h"
#include "TimeParameterInterpolator.h"

class BasicForceMeshDeformationTFShader : public MeshTransformFeedbackShader{

private:
	vec3 forceDirection;
	float forceIntensity;
	vec3 staticPoint;
	float staticStrength;
	TimeParameterInterpolator* forceIntensityInterpolator;

	GLSLUniform* staticPointUniform;
	GLSLUniform* staticStrengthUniform;
	GLSLUniform* forceDirectionUniform;	
	GLSLUniform* forceIntensityUniform;

public:
	BasicForceMeshDeformationTFShader();
	virtual ~BasicForceMeshDeformationTFShader();

	// Se declaran que VBOs del MeshBuffers se escribiran en el output del transform feedback (se declaran en el transformFeedbackObject)
	void declareFeedbackInputsAndOutputs(MeshBuffers* meshBuffersInput, MeshBuffers* meshBuffersOutput);

	// Se llama para generar la transformacion. Es analogo a draw en los shaders con rasterizador
	void execute();	

	// Inicia el shader de transformacion
	void init();

	//Define la direccion dela fuerza aplicada
	void setForceDirection(vec3 direction){ this->forceDirection = glm::normalize(direction); }
	//Define la intensidad de la fuerza aplicada
	void setForceIntensity(float intensity){ this->forceIntensityInterpolator->setMaxValue(intensity); }

	//Define la posicion del punto de agarre (es decir donde la fuerza no actuara o tendra un menor aporte)
	void setStaticPoint(vec3 position){ this->staticPoint = position; }
	//Define la fuerza del punto de agarre (cuanta mayor fuerza, mayor sera el area cubierta por el punto de agarre)
	void setStaticStrength(float strength){ this->staticStrength = strength; }

	// Si se desea animacion se debe setear el tiempo que esta durara
	void setAnimationTime(unsigned int time);

	// Setea si al hacer loop la animacion debe volver al principio o invertirse el sentido
	void isAnimationReverseLoopEnabled(bool enabled){ this->forceIntensityInterpolator->setInverseAnimationEnabled(enabled); }
	
	// Para prender o apagar la animacion
	void playAnimation(){ this->forceIntensityInterpolator->setPlay(); }
	void stopAnimation(){ this->forceIntensityInterpolator->setStop(); }
};