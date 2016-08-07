/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "BlurStageTemplate.h"
#include "ApiTexture.h"
#include "DepthOfFieldShader.h"
#include "GeometryStage.h"

class DepthOfFieldStage : public BlurStageTemplate{

protected:
	DepthOfFieldShader* depthOfFieldShader;
	GeometryStage* geometryStage;
	float depthAtBlurStart;
	bool blurToFront;
	float blurFallOff;

private:
	// Inicializa el shader program(s) a utilizar por el stage
	void intiateShaderProgram();

public:
	// Define el nombre del stage. Esto se utiliza para identificar al stage por ejemplo en el profiling
	string getName(){ return "DepthOfFieldStage"; }

	// Tiene como dependencia alguna stage que deje una unica textura
	DepthOfFieldStage(GeometryStage* geometryStage, BackBufferWriterStage* previousStage, int imageWidth, int imageHeight, float blurFactor, float depthAtBlurStart, bool blurToFront);
	
	// Define sus outputs
	void configureStageOutputs();

	// Renderiza el render stage actual
	virtual void render(bool isTheLastStage);

	// Agrega al vector de inputs los inputs de este Render Stage particular
	 void addStageInputs(list<OutputResource*>* inputs);

	// Actualiza la informacion de las texturas de salida
	virtual void updateOutputData();

	// Seteo en que punto debe comenzar el blur. Antes de ese punto no habra blur
	virtual void setDepthAtBlurStart(float depth){ this->depthAtBlurStart = depth; }

	// Defino si el blur se aplicara hacia la camara o hacia al lado opuesto (a partir del DepthAtBlurStart)
	virtual void setBlurToFront(bool isToFront){ this->blurToFront = isToFront; }

	// El factor de decaimiento del blur (es decir que tan rapido llegara el blur a su maximo una vez alcanzado depthAtBlurStart)
	virtual void setBlurFalloff(float value){ this->blurFallOff = value; }
};