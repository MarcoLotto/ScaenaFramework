/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "BlurStageTemplate.h"
#include "ApiTexture.h"
#include "BloomShader.h"

class BloomStage : public BlurStageTemplate{

private:
	BloomShader* bloomShader;
	float lumThreshold;
	FrameBuffer* thirdFrameBuffer;

	// Inicializa el shader program(s) a utilizar por el stage
	void intiateShaderProgram();
		
public:
	// Define el nombre del stage. Esto se utiliza para identificar al stage por ejemplo en el profiling
	string getName(){ return "BloomStage"; }

	// Tiene como dependencia alguna stage que deje una unica textura
	BloomStage(BackBufferWriterStage* previousStage, int screenWidth, int screenHeight, float blurFactor);
	
	// Define sus outputs
	void configureStageOutputs();

	// Renderiza el render stage actual
	void render(bool isTheLastStage);

	// Actualiza la informacion de las texturas de salida
	void updateOutputData();

	// Setea el lum thresshold, que indica cuando se debe aplicar bloom a un fragmento
	void setLumThreshold(float factor){ this->lumThreshold = factor; }
};