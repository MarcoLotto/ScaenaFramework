/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "BlurStage.h"
#include "VarianceShadowMapStage.h"

class ShadowBlurStage : public BlurStage{

private:
	FrameBufferRequest* configureShadowFBORequestPass1(int width, int height);
	FrameBufferRequest* configureShadowFBORequestPass2(int width, int height);
	VarianceShadowMapStage* shadowStage;
	bool blurPrebankLoaded;
	FrameBufferRequest* fboRequestPass1;
	FrameBufferRequest* fboRequestPass2;

public:
	// Tiene como dependencia alguna stage que deje una unica textura
	ShadowBlurStage(VarianceShadowMapStage* previousStage, int imageWidth, int imageHeight, float blurFactor);
	virtual ~ShadowBlurStage();

	// Define el nombre del stage. Esto se utiliza para identificar al stage por ejemplo en el profiling
	string getName(){ return "ShadowBlurStage"; }

	// Renderiza el render stage actual
	virtual void render(bool isTheLastStage);

	// Actualiza la informacion de las texturas de salida
	virtual void updateOutputData();
};