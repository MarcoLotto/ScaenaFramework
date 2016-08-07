/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "BackBufferWriterStage.h"
#include "ApiTexture.h"
#include "UIController.h"

class UserInterfaceStage : public BackBufferWriterStage{

private:
	FrameBufferRequest* configureFBORequest(int width, int height);
	FrameBufferRequest* fboRequest;
	FrameBuffer* uiDataFrameBuffer;  // Para renderizar solo la interfaz de usuario
	FrameBuffer* mergeFrameBuffer;   // Para mergear la UI con el stage previo
	BackBufferWriterStage* previousStage;
	UIController* uiController;
    bool cleanScreen;
	
public:
	// Tiene como dependencia alguna stage que deje una unica textura
	UserInterfaceStage(BackBufferWriterStage* previousStage, int screenWidth, int screenHeight);
    
    // La otra opcion es no usar un stage anterior, si no que renderizar directamente a pantalla (el ultimo parametro indica si borrar o no la pantalla)
    UserInterfaceStage(int screenWidth, int screenHeight, bool cleanScreen);
	
    virtual ~UserInterfaceStage();

	// Define el nombre del stage. Esto se utiliza para identificar al stage por ejemplo en el profiling
	string getName(){ return "UserInterfaceStage"; }

	// Define sus outputs
	void configureStageOutputs();

	// Renderiza el render stage actual
	void render(bool isTheLastStage);

	// Agrega al vector de inputs los inputs de este Render Stage particular
	void addStageInputs(list<OutputResource*>* inputs);

	// Actualiza la informacion de las texturas de salida
	void updateOutputData();

	// Tiene como salida el back buffer con la UI renderizada
	OutputResource* getBackBuffer(){ return this->getOutputs()->at(0); };	

	// Setea la dependencia (tiene que ser una stage que deje una unica textura)
	void setPreviousStage(BackBufferWriterStage* stage){ this->previousStage = stage; }

	// Seteo el controlador de user interface
	void setUiController(UIController* controller){ this->uiController = controller; }
};