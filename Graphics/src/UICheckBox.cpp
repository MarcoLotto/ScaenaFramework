/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UICheckBox.h"

UICheckBox::UICheckBox(const char* filename, UIController* uiController) : UIElement(uiController){
	//La ruta de la imagen a cargar
	this->filename = filename;

	// Por defecto no estara chequeado
	this->checked = false;

	//Como no es un form, tengo que inicializar a mano
	this->initiate();
}

//Incializacion de los estados del elemento 
void UICheckBox::initiateStates(){
	this->uncheckedState = UIFactory::createPartialTextureState(vec2(0.0f), 0.0f, vec2(1.0f, 1.0f), 1.0f, 
		this->filename, vec2(1.0f/3.0f, 1.0f), vec2(0.0f));
	this->pressState = UIFactory::createPartialTextureState(vec2(0.0f), 0.0f, vec2(1.0f, 1.0f), 1.0f, 
		this->filename, vec2(1.0f/3.0f, 1.0f), vec2(1.0f, 0.0f));
	this->checkedState = UIFactory::createPartialTextureState(vec2(0.0f), 0.0f, vec2(1.0f, 1.0f), 1.0f, 
		this->filename, vec2(1.0f/3.0f, 1.0f), vec2(2.0f, 0.0f));		
}

//Inicializacion de los subelementos
void UICheckBox::initiateElements(){	
}

void UICheckBox::bindElementsToStates(){
	this->internalState->makeStateTransition(this->uncheckedState);	
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void UICheckBox::handleEvents(){
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;
		
	//Compruebo de que elemento se trata el evento y lo proceso
	if((this->onFocusEvent.getStateCode() == ONFOCUS_UIEVENTCODE)||(this->onFocusEvent.getStateCode() == ONFOCUSHOLD_UIEVENTCODE)){
		this->processPressEvent();	
	}
	else if(this->onFocusEvent.getStateCode() == ONFOCUSRELEASE_UIEVENTCODE){
		this->processReleaseEvent();		
	}
	//Vacio el evento (No me interesa en este caso mantener estados de hold ni nada de eso)
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

void UICheckBox::processPressEvent(){
	this->internalState->makeStateTransition(this->pressState);
}

void UICheckBox::processReleaseEvent(){
	if(this->isChecked()){
		this->internalState->makeStateTransition(this->uncheckedState);
		this->checked = false;
	}
	else{
		this->internalState->makeStateTransition(this->checkedState);
		this->checked = true;
	}
}

void UICheckBox::setChecked(bool value){ 
	this->checked = value; 
	//Actualizo la imagen del checkbox
	if(this->checked) 
		this->internalState->makeStateTransition(this->checkedState);
	else 
		this->internalState->makeStateTransition(this->uncheckedState);
}

UICheckBox::~UICheckBox(){
	this->deleteState(this->uncheckedState);
	this->deleteState(this->pressState);
	this->deleteState(this->checkedState);	
}
