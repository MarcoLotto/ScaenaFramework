#include "PipelineSelectorForm.h"
#include "Logger.h"
#include "VideoConfiguration.h"

#define HIDE_TRANSITION_END_CALLBACK_ID 0
#define FONT_SIZE 0.14f
#define BUTTON_WIDTH 0.6f
#define BUTTON_HEIGHT 0.1f
#define BUTTON_X_POSITION 0.02f
#define BUTTON_TEXTURE "Textures/UI/button1.png"
#define BUTTON_FONT "Textures/UI/font2.png"
#define FONT_POS_X 0.05f
#define FONT_POS_Y 0.02f

PipelineSelectorForm::PipelineSelectorForm(UIController* uiController) : UIForm(uiController){
	this->initiate();	
	this->visible = false;
	this->setVisible(true);
}

//Incializacion de los estados del elemento 
void PipelineSelectorForm::initiateStates(){			
	this->showedState = UIFactory::createState(vec2(0.02f, 0.15f), 0.0f, this->getFormSize(), 1.0f, "Textures/UI/FormBackground2.png");
	this->hidedState = UIFactory::createState(vec2(-0.5f, 0.15f), 0.0f, this->getFormSize(), 0.0f, "Textures/UI/FormBackground2.png");
}
//Inicializacion de los subelementos
void PipelineSelectorForm::initiateElements(){	
	vec2 buttonSize = vec2(BUTTON_WIDTH, BUTTON_HEIGHT);
	vec2 fontPosition = vec2(FONT_POS_X, FONT_POS_Y);
	this->button1 = UIFactory::createButton(vec2(BUTTON_X_POSITION, 0.02f), 0.0f, buttonSize, "Forward shader", FONT_SIZE, fontPosition, BUTTON_TEXTURE, BUTTON_FONT, this->uiController);
	this->button2 = UIFactory::createButton(vec2(BUTTON_X_POSITION, 0.12f), 0.0f, buttonSize, "Forward shader + Bloom", FONT_SIZE, fontPosition, BUTTON_TEXTURE, BUTTON_FONT, this->uiController);
	this->button3 = UIFactory::createButton(vec2(BUTTON_X_POSITION, 0.22f), 0.0f, buttonSize, "Forward + Blur", FONT_SIZE, fontPosition, BUTTON_TEXTURE, BUTTON_FONT, this->uiController);
	this->button4 = UIFactory::createButton(vec2(BUTTON_X_POSITION, 0.32f), 0.0f, buttonSize, "Deferred shader", FONT_SIZE, fontPosition, BUTTON_TEXTURE, BUTTON_FONT, this->uiController);
	this->button5 = UIFactory::createButton(vec2(BUTTON_X_POSITION, 0.42f), 0.0f, buttonSize, "Deferred + Far depth of field", FONT_SIZE, fontPosition,BUTTON_TEXTURE, BUTTON_FONT, this->uiController);
	this->button6 = UIFactory::createButton(vec2(BUTTON_X_POSITION, 0.52f), 0.0f, buttonSize, "Deferred + Near depth of field", FONT_SIZE, fontPosition, BUTTON_TEXTURE, BUTTON_FONT, this->uiController);
}

void PipelineSelectorForm::bindElementsToStates(){
	// Asigno a cada estado los elementos que contendran
	//this->state1->addElement(this->gameIcon);
	this->showedState->addElement(this->button1);
	this->showedState->addElement(this->button2);
	this->showedState->addElement(this->button3);
	this->showedState->addElement(this->button4);
	
	this->hidedState->addElement(this->button1);
	this->hidedState->addElement(this->button2);
	this->hidedState->addElement(this->button3);
	this->hidedState->addElement(this->button4);
	
	// Solo mostramos las opciones de Depth of field si tenemos la version correcta de OpenGL
	if(VideoConfiguration::getInstance()->isGlsl400Avaible()){
		this->showedState->addElement(this->button5);
		this->showedState->addElement(this->button6);
		this->hidedState->addElement(this->button5);
		this->hidedState->addElement(this->button6);
	}		
	//Inicializo el estado inicial
	this->internalState->makeStateTransition(this->hidedState);	
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void PipelineSelectorForm::handleEvents(){
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;

	//Compruebo de que elemento se trata el evento y lo proceso	
	if( this->onFocusEvent.getStateCode() == ONFOCUSRELEASE_UIEVENTCODE ){

		this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);

		if(this->onFocusEvent.getAfectedElement() == this->button1){
			this->processButton1Event();	
		}
		else if(this->onFocusEvent.getAfectedElement() == this->button2){
			this->processButton2Event();	
		}
		else if(this->onFocusEvent.getAfectedElement() == this->button3){
			this->processButton3Event();	
		}
		else if(this->onFocusEvent.getAfectedElement() == this->button4){
			this->processButton4Event();	
		}
		else if(this->onFocusEvent.getAfectedElement() == this->button5){
			this->processButton5Event();	
		}
		else if(this->onFocusEvent.getAfectedElement() == this->button6){
			this->processButton6Event();	
		}
	}
	//Vacio el evento
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

PipelineSelectorForm::~PipelineSelectorForm(){
	delete this->showedState;
	delete this->hidedState;
	delete this->button1;
	delete this->button2;
	delete this->button3;
	delete this->button4;
	delete this->button5;
	delete this->button6;
}

void PipelineSelectorForm::processButton1Event(){
	// Pipeline 1 (Forward shader)
	this->setVisible(false);
	this->selectedPipeline = PipelineSelectionIndex::p1;
}

void PipelineSelectorForm::processButton2Event(){
	// Pipeline 2 (Forward shader + bloom)
	this->setVisible(false);
	this->selectedPipeline = PipelineSelectionIndex::p2;
}

void PipelineSelectorForm::processButton3Event(){
	// Pipeline 6 (Forward + Blur)
	this->setVisible(false);
	this->selectedPipeline = PipelineSelectionIndex::p3;
}

void PipelineSelectorForm::processButton4Event(){
	// Pipeline 3 (Deferred shader)
	this->setVisible(false);
	this->selectedPipeline = PipelineSelectionIndex::p4;
}

void PipelineSelectorForm::processButton5Event(){
	// Pipeline 4 (Deferred + far depth of field)
	this->setVisible(false);
	this->selectedPipeline = PipelineSelectionIndex::p5;
}

void PipelineSelectorForm::processButton6Event(){
	// Pipeline 5 (Deferred + near depth of field)
	this->setVisible(false);
	this->selectedPipeline = PipelineSelectionIndex::p6;
}

void PipelineSelectorForm::setVisible(bool visible){ 
	if(visible){
		this->visible = true;
		this->internalState->makeStateTransition(this->showedState, 550, false);
	}
	else{
		this->internalState->makeStateTransition(this->hidedState, 550, false, this, HIDE_TRANSITION_END_CALLBACK_ID);
	}
}

void PipelineSelectorForm::onScreenChange(){
	// TODO: Manejar llamadas a otras pantallas aca
}

void PipelineSelectorForm::onCallback(int callbackId){
	switch(callbackId){	
	case HIDE_TRANSITION_END_CALLBACK_ID:
		this->visible = false;
		this->onScreenChange();	
		break;
	}
}

// Esto es para que se agrande o no el form si hay mas opciones
vec2 PipelineSelectorForm::getFormSize(){
	if(VideoConfiguration::getInstance()->isGlsl400Avaible()){
		return vec2(0.65f, 0.65f);
	}
	return vec2(0.65f, 0.45f);
}