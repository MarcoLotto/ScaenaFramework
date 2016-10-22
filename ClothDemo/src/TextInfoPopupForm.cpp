/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "TextInfoPopupForm.h"
#include "Logger.h"

#define MAX_LETTERS_PER_LINE 45
TextInfoPopupForm::TextInfoPopupForm(UIController* uiController) : UIForm(uiController){	
	this->callbackElement = NULL;
	this->initiate();	
}

//Incializacion de los estados del elemento 
void TextInfoPopupForm::initiateStates(){		
	this->showedState = UIFactory::createState(vec2(0.2f, 0.25f), 0.0f, vec2(0.6f, 0.3f), 1.0f, "Textures/UI/FormBackground2.png");
	this->hidedState = UIFactory::createState(vec2(0.2f, -1.0f), 0.0f, vec2(0.6f, 0.3f), 0.0f, "Textures/UI/FormBackground2.png");
}
//Inicializacion de los subelementos
void TextInfoPopupForm::initiateElements(){
	this->textLine1 = UIFactory::createLabel("", vec2(0.025f, 0.03f), 0.12f, "Textures/UI/font2.png", this->uiController);
	this->textLine2 = UIFactory::createLabel("", vec2(0.025f, 0.09f), 0.12f, "Textures/UI/font2.png", this->uiController);
	this->textLine3 = UIFactory::createLabel("", vec2(0.025f, 0.15f), 0.12f, "Textures/UI/font2.png", this->uiController);
	this->button1 = UIFactory::createButton(vec2(0.2, 0.21f), 0.0f, vec2(0.2f, 0.08f), "Aceptar", 0.13f, vec2(0.05f, 0.015f), "Textures/UI/Button1.png", "Textures/UI/font2.png", this->uiController);
}

void TextInfoPopupForm::bindElementsToStates(){
	// Asigno a cada estado los elementos que contendran	
	this->showedState->addElement(this->textLine1);
	this->showedState->addElement(this->textLine2);
	this->showedState->addElement(this->textLine3);
	this->showedState->addElement(this->button1);
			
	//Inicializo el estado inicial
	this->internalState->makeStateTransition(this->hidedState);	
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void TextInfoPopupForm::handleEvents(){
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;

	//Compruebo de que elemento se trata el evento y lo proceso	
	if( this->onFocusEvent.getStateCode() == ONFOCUSRELEASE_UIEVENTCODE ){
		if(this->onFocusEvent.getAfectedElement() == this->button1){
			this->processButton1Event();
		}
	}
	//Vacio el evento
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

TextInfoPopupForm::~TextInfoPopupForm(){
	delete this->showedState;
	delete this->hidedState;
	delete this->textLine1;
	delete this->textLine2;
	delete this->textLine3;
	delete this->button1;	
}

void TextInfoPopupForm::processButton1Event(){
	this->setVisible(false);

	// Le avisamos al elemento callback que se cerro el popup
	if(this->callbackElement != NULL){
		Callbackable* aux = this->callbackElement;
		this->callbackElement = NULL;
		aux->onCallback(POPUP_CLOSED_CALLBACK_ID);
	}
}

void TextInfoPopupForm::setVisible(bool visible){ 
	// No hace falta setear la propiedad de visible porque el estado final termina con transparencia 0 (y scaena no lo dibuja)
	if(visible){
		this->internalState->makeStateTransition(this->showedState, 750);
	}
	else{
		this->internalState->makeStateTransition(this->hidedState, 750);
	}
}

void TextInfoPopupForm::setInfoText(string text){
	if(text.size() < MAX_LETTERS_PER_LINE){
		this->textLine1->setText(StringUtils::substring(text, 0, MAX_LETTERS_PER_LINE));
		this->textLine2->setText("");
		this->textLine3->setText("");
	}
	else if(text.size() < 2*MAX_LETTERS_PER_LINE){
		this->textLine1->setText(StringUtils::substring(text, 0, MAX_LETTERS_PER_LINE));
		this->textLine2->setText(StringUtils::substring(text, MAX_LETTERS_PER_LINE, MAX_LETTERS_PER_LINE));
		this->textLine3->setText("");
	}
	else{
		this->textLine1->setText(StringUtils::substring(text, 0, MAX_LETTERS_PER_LINE));
		this->textLine2->setText(StringUtils::substring(text, MAX_LETTERS_PER_LINE, MAX_LETTERS_PER_LINE));
		this->textLine3->setText(StringUtils::substring(text, 2*MAX_LETTERS_PER_LINE, MAX_LETTERS_PER_LINE));
	}
}

void TextInfoPopupForm::setInfoTitle(string text){
	this->textLine3->setText(this->textLine2->getText());
	this->textLine2->setText(this->textLine1->getText());
	this->textLine1->setText(text);
}

void TextInfoPopupForm::setCallbackElement(Callbackable* element){
	this->callbackElement = element;
}