/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "BackBufferWriterStage.h"
#include "ApiTexture.h"
#include "BlurShader.h"

class BlurStageTemplate : public BackBufferWriterStage{
	
protected:
	FrameBufferRequest* configureFBORequest(int width, int height);
	FrameBufferRequest* fboRequest1;
	FrameBuffer* firstFrameBuffer;
	FrameBuffer* secondFrameBuffer;
	BackBufferWriterStage* previousStage;
	
	float* weights;
	float* posIncrement;
	float weightSum;
	
	void initWeights(float blurFactor);
	float gauss(int x, float sigma);
		
public:
	BlurStageTemplate();
	virtual ~BlurStageTemplate();

	// Agrega al vector de inputs los inputs de este Render Stage particular
	 virtual void addStageInputs(list<OutputResource*>* inputs);

	// Tiene como salida el back buffer con la UI renderizada
	OutputResource* getBackBuffer(){ return this->getOutputs()->at(0); };	

	// Setea la dependencia (tiene que ser una stage que deje una unica textura)
	void setPreviousStage(BackBufferWriterStage* stage){ this->previousStage = stage; }

	// Cambia el factor de blur para obtener diferentes efectos. Esto puede cambiarse en
	// tiempo de ejecucion sin mucho problema, no es tan costoso.
	void changeBlurFactor(float sigma);
};