/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "TeamBuilderDataForm.h"
#include "Logger.h"
#include "MenuStatusBarForm.h"
#include "FormsManager.h"
#include "TextInfoPopupForm.h"
#include "VirtualKeyboardForm.h"
#include "WIndowConnector.h"

#define HIDE_TRANSITION_END_CALLBACK_ID 0

TeamBuilderDataForm::TeamBuilderDataForm(UIController* uiController) : UIForm(uiController){
	this->initiate();	
	this->visible = false;
}

//Incializacion de los estados del elemento 
void TeamBuilderDataForm::initiateStates(){			
	this->showedState = UIFactory::createState(vec2(0.25f, 0.15f), 0.0f, vec2(0.5f, 0.6f), 1.0f, "Textures/UI/FormBackground1.png");
	this->hidedState = UIFactory::createState(vec2(0.25f, -0.75f), 0.0f, vec2(0.5f, 0.6f), 0.0f, "Textures/UI/FormBackground1.png");
}
//Inicializacion de los subelementos
void TeamBuilderDataForm::initiateElements(){	
	this->input = UIFactory::createTextBox(vec2(0.01f, 0.05f), 0.0f, vec2(0.48f, 0.25f), "Nombre", 0.14f, 30, 30, "./Textures/UI/textbox1.png", this->uiController);
	this->continueButton = UIFactory::createButton(vec2(0.15, 0.45f), 0.0f, vec2(0.2f, 0.1f), "Crear!", 0.13f, vec2(0.055f, 0.024f), "./Textures/UI/Button1.png", "Textures/UI/font2.png", this->uiController);
	this->uiCheckbox = UIFactory::createCheckBox(vec2(0.01f, 0.3f), 0.0f, vec2(0.04f, 0.05f), "./Textures/UI/checkbox1.png", this->uiController);
	this->uiCheckbox->setChecked(true);
	this->checkboxLabel = UIFactory::createLabel("Checkbox text", vec2(0.051f, 0.31f), 0.12f, "Textures/UI/font2.png", this->uiController); 
}

void TeamBuilderDataForm::bindElementsToStates(){
	// Asigno a cada estado los elementos que contendran
	this->showedState->addElement(this->input);
	this->showedState->addElement(this->continueButton);
	this->showedState->addElement(this->uiCheckbox);
	this->showedState->addElement(this->checkboxLabel);
	
	this->hidedState->addElement(this->input);
	this->hidedState->addElement(this->continueButton);
	this->hidedState->addElement(this->uiCheckbox);
	this->hidedState->addElement(this->checkboxLabel);
		
	//Inicializo el estado inicial
	this->internalState->makeStateTransition(this->hidedState);	
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void TeamBuilderDataForm::handleEvents(){
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;

	//Compruebo de que elemento se trata el evento y lo proceso	
	if( this->onFocusEvent.getStateCode() == ONFOCUSRELEASE_UIEVENTCODE ){
		if(this->onFocusEvent.getAfectedElement() == this->input){
			this->processInputEvent();	
		}
		else if(this->onFocusEvent.getAfectedElement() == this->continueButton){
			this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
			this->processContinueButtonEvent();	
		}
	}
	//Vacio el evento
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

TeamBuilderDataForm::~TeamBuilderDataForm(){
	delete this->showedState;
	delete this->hidedState;
	delete this->continueButton;
	delete this->input;
	delete this->uiCheckbox;
	delete this->checkboxLabel;
}

void TeamBuilderDataForm::processInputEvent(){
	// Abrimos el teclado virtual
	VirtualKeyboardForm* keyboard = (VirtualKeyboardForm*) FormsManager::getInstance()->virtualKeyboardForm;
	keyboard->setFocusedElement(this->getOnFocusEvent()->getAfectedElement());
	keyboard->setVisible(true);
	//WindowConnector::openKeyboard();
}

bool TeamBuilderDataForm::runFieldValidations(){
	TextInfoPopupForm* infoPopup = (TextInfoPopupForm*) FormsManager::getInstance()->textInfoPopupForm;	
	if(this->dataTransfer.teamName.empty()){		
		infoPopup->setInfoText("El nombre no puede estar vacio");
		infoPopup->setInfoTitle("Error en campo");
		infoPopup->setVisible(true);
		return false;
	}
	return true;
		
}

void TeamBuilderDataForm::processContinueButtonEvent(){
	// Primero forzamos a cerrar el teclado
	WindowConnector::closeKeyboard();
	
	// TODO: Estaria mejor que apareciera un pop-up con confirmacion

	// Nos guardamos la data de este form
	this->dataTransfer.teamName = this->input->getText();
	
	// Corremos validaciones
	if(this->runFieldValidations()){
		// Terminamos con los builders
		((TextInfoPopupForm*) FormsManager::getInstance()->textInfoPopupForm)->setInfoText("La indumentaria ha sido creada");
		((TextInfoPopupForm*) FormsManager::getInstance()->textInfoPopupForm)->setInfoTitle("Exito!!");		
		FormsManager::getInstance()->textInfoPopupForm->setVisible(true);
	}
}

void TeamBuilderDataForm::setVisible(bool visible){ 
	if(visible){
		this->visible = true;
		this->internalState->makeStateTransition(this->showedState, 550, false);
	}
	else{
		// Mandamos a ocultar este form y el form que te ofrec crear un nuevo team
		this->internalState->makeStateTransition(this->hidedState, 550, false, this, HIDE_TRANSITION_END_CALLBACK_ID);
	}
}

void TeamBuilderDataForm::onScreenChange(){
	// TODO: Manejar llamadas a otras pantallas aca
}

void TeamBuilderDataForm::onCallback(int callbackId){
	switch(callbackId){	
	case HIDE_TRANSITION_END_CALLBACK_ID:
		this->visible = false;
		this->onScreenChange();	
		break;
	}
}

