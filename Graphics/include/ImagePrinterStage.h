/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "BackBufferWriterStage.h"
#include "ApiTexture.h"

/**
// Esta clase se encarga de dado un back buffer stage, dibujarlo a pantalla (o a otro frame).
// Esto se puede utilizar por ejemplo para redimensionar imagenes (por ejemplo para dibujar en resoluciuones menores a la de la pantalla).
**/
class ImagePrinterStage : public BackBufferWriterStage{

private:
	FrameBufferRequest* configureFBORequest(int width, int height);
	FrameBufferRequest* fboRequest;	
	FrameBuffer* frameBuffer; 
	BackBufferWriterStage* previousStage;	
	
public:
	// Tiene como dependencia alguna stage que deje una unica textura
	ImagePrinterStage(BackBufferWriterStage* previousStage, int screenWidth, int screenHeight);
	virtual ~ImagePrinterStage();

	// Define el nombre del stage. Esto se utiliza para identificar al stage por ejemplo en el profiling
	string getName(){ return "ImagePrinterStage"; }

	// Define sus outputs
	void configureStageOutputs();

	// Renderiza el render stage actual
	void render(bool isTheLastStage);

	// Agrega al vector de inputs los inputs de este Render Stage particular
	void addStageInputs(list<OutputResource*>* inputs);

	// Actualiza la informacion de las texturas de salida
	void updateOutputData();

	// Tiene como salida el back buffer con la imagen copiada
	OutputResource* getBackBuffer(){ return this->getOutputs()->at(0); };	

	// Setea la dependencia (tiene que ser una stage que deje una unica textura)
	void setPreviousStage(BackBufferWriterStage* stage){ this->previousStage = stage; }	
};