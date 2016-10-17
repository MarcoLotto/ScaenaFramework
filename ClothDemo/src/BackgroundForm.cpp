/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "BackgroundForm.h"
#include "Logger.h"
#include "FormsManager.h"

BackgroundForm::BackgroundForm(UIController* uiController) : UIForm(uiController){	
	this->initiate();	
}

//Incializacion de los estados del elemento 
void BackgroundForm::initiateStates(){		
	this->state = UIFactory::createState(vec2(0.0f, 0.0f), 0.0f, vec2(1.0f, 1.0f), 1.0f, "Textures/UI/background.png");
}
//Inicializacion de los subelementos
void BackgroundForm::initiateElements(){
}

void BackgroundForm::bindElementsToStates(){		
	//Inicializo el estado inicial
	this->internalState->makeStateTransition(this->state);
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void BackgroundForm::handleEvents(){
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;

	//Vacio el evento
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

BackgroundForm::~BackgroundForm(){
	delete this->state;
}

