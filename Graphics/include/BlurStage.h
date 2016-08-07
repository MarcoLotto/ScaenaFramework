/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "BlurStageTemplate.h"
#include "ApiTexture.h"
#include "BlurShader.h"

class BlurStage : public BlurStageTemplate{

protected:
	BlurShader* blurShader;

private:
	// Inicializa el shader program(s) a utilizar por el stage
	void intiateShaderProgram();

public:
	// Define el nombre del stage. Esto se utiliza para identificar al stage por ejemplo en el profiling
	string getName(){ return "BlurStage"; }

	// Tiene como dependencia alguna stage que deje una unica textura
	BlurStage(BackBufferWriterStage* previousStage, int imageWidth, int imageHeight, float blurFactor);
	
	// Define sus outputs
	void configureStageOutputs();

	// Renderiza el render stage actual
	virtual void render(bool isTheLastStage);

	// Actualiza la informacion de las texturas de salida
	virtual void updateOutputData();
};