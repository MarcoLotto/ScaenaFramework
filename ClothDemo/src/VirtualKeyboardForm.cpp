/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "VirtualKeyboardForm.h"
#include "Logger.h"
#include "MenuStatusBarForm.h"
#include "ProgramParameters.h"
#include "ObjectFactory.h"
#include "FormsManager.h"

#define HIDE_TRANSITION_END_CALLBACK_ID 0
#define SHOW_TRANSITION_END_CALLBACK_ID 1

VirtualKeyboardForm::VirtualKeyboardForm(UIController* uiController) : UIForm(uiController){
	this->initiate();	
	this->visible = false;
    this->inStateTransition = false;
}

//Incializacion de los estados del elemento 
void VirtualKeyboardForm::initiateStates(){			
	this->showedState = UIFactory::createState(vec2(0.0f, 0.58f), 0.0f, vec2(1.0f, 0.42f), 1.0f, "Textures/UI/FormBackground2.png");
	this->hidedState = UIFactory::createState(vec2(0.0f, 1.0f), 0.0f, vec2(1.0f, 0.42f), 0.0f, "Textures/UI/FormBackground2.png");
}
//Inicializacion de los subelementos
void VirtualKeyboardForm::initiateElements(){	
	this->keyboard = UIFactory::createVirtualKeyboard(vec2(0.0f, 0.02f), 0.0f, vec2(0.3f, 0.3f), "./Textures/UI/Button2.png", "./Textures/UI/Button1.png", this->uiController);
}

void VirtualKeyboardForm::bindElementsToStates(){
	// Asigno a cada estado los elementos que contendran
	this->showedState->addElement(this->keyboard);	
	this->hidedState->addElement(this->keyboard);
		
	//Inicializo el estado inicial
	this->internalState->makeStateTransition(this->hidedState);	
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void VirtualKeyboardForm::handleEvents(){
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;

	//Compruebo de que elemento se trata el evento y lo proceso	
	if( this->onFocusEvent.getStateCode() == ONFOCUSRELEASE_UIEVENTCODE ){

		this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
		
		if(this->onFocusEvent.getAfectedElement() == this->keyboard){
			this->processKeyboardEvent();	
		}			
	}
	//Vacio el evento
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

void VirtualKeyboardForm::processKeyboardEvent(){
	if(this->keyboard->isLastKeyEnter()){
		this->setVisible(false);
	}
}

VirtualKeyboardForm::~VirtualKeyboardForm(){
	delete this->showedState;
	delete this->hidedState;
	delete this->keyboard;	
}

void VirtualKeyboardForm::setVisible(bool visible){ 
	if(!this->visible && visible && !this->inStateTransition){
		this->visible = true;
		this->uiController->giveFocusTo(this);
		this->internalState->makeStateTransition(this->showedState, 250, false, this, SHOW_TRANSITION_END_CALLBACK_ID);
        this->inStateTransition = true;
	}
	else if(this->visible && !visible && !this->inStateTransition){
		this->internalState->makeStateTransition(this->hidedState, 250, false, this, HIDE_TRANSITION_END_CALLBACK_ID);
        this->inStateTransition = true;
	}	
}

void VirtualKeyboardForm::onCallback(int callbackId){
	switch(callbackId){	
	case HIDE_TRANSITION_END_CALLBACK_ID:
		this->visible = false;
        this->inStateTransition = false;
		break;
    case SHOW_TRANSITION_END_CALLBACK_ID:
        this->inStateTransition = false;
        break;
	}
}

void VirtualKeyboardForm::onLostFocus(){
	// Si perdimos el foco, ocultamos el teclado
	this->setVisible(false);
}
