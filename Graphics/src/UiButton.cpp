/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include <string.h>
#include "UiButton.h"

#define DEFAULT_FONT "./Textures/UI/font2.png"

UiButton::UiButton(const char* backgroundFilename, UIController* uiController) : UIElement(uiController){
	this->initButton(backgroundFilename, DEFAULT_FONT, vec2(0.0f), "", vec2(0.0f), vec2(0.0f), uiController);
}

UiButton::UiButton(const char* backgroundFilename, const char* font, vec2 textPos, UIController* uiController) : UIElement(uiController){
	this->initButton(backgroundFilename, font, textPos, "", vec2(0.0f), vec2(0.0f), uiController);
}

UiButton::UiButton(const char* backgroundFilename, const char* font, vec2 textPos, const char* imageFilename, vec2 imgPos, vec2 imgScale, UIController* uiController) : UIElement(uiController){
	this->initButton(backgroundFilename, font, textPos, imageFilename, imgPos, imgScale, uiController);
}

UiButton::UiButton(const char* backgroundFilename, const char* imageFilename, vec2 imgPos, vec2 imgScale, UIController* uiController) : UIElement(uiController){
	this->initButton(backgroundFilename, "", vec2(0.0f), imageFilename, imgPos, imgScale, uiController);
}

void UiButton::initButton(const char* backgroundFilename, const char* font, vec2 textPos, const char* imageFilename, vec2 imgPos, vec2 imgScale, UIController* uiController){
	//La ruta de la imagen a cargar
	this->filename = backgroundFilename;
	this->font = font;
	this->textPos = textPos;	
	this->image = NULL;	
	this->label = NULL;
	this->imgPos = imgPos;
	this->imgScale = imgScale;
	this->imageFilename = imageFilename;

	//Como no es un form, tengo que inicializar a mano
	this->initiate();
}

//Incializacion de los estados del elemento 
void UiButton::initiateStates(){
	this->basicState = UIFactory::createPartialTextureState(vec2(0.0f), 0.0f, vec2(1.0f, 1.0f), 1.0f, 
		this->filename, vec2(1.0f/3.0f, 1.0f), vec2(0.0f));
	this->pressState = UIFactory::createPartialTextureState(vec2(0.0f), 0.0f, vec2(1.0f, 1.0f), 1.0f, 
		this->filename, vec2(1.0f/3.0f, 1.0f), vec2(1.0f, 0.0f));
	this->mouseOnState = UIFactory::createPartialTextureState(vec2(0.0f), 0.0f, vec2(1.0f, 1.0f), 1.0f, 
		this->filename, vec2(1.0f/3.0f, 1.0f), vec2(2.0f, 0.0f));		
}

//Inicializacion de los subelementos
void UiButton::initiateElements(){
	//Los valores de tamaño y separacion entre letras no son importantes aqui
	if(this->usesLabel()){
		this->label = UIFactory::createLabel("", this->textPos, 0.1f, this->font, this->uiController);
	}
	// Si se solicito que el boton tenga una imagen junto al texto, creamos la imagen
	if(this->usesImage()){
		this->image = UIFactory::createStaticImage(this->imgPos, 0.0f, this->imgScale, 1.0f, this->imageFilename, this->uiController);  
	}
}

void UiButton::bindElementsToStates(){
	// Si estamos usando una imagen para el boton, agregamos la imagen a todos los estados
	if(this->usesImage()){
		this->basicState->addElement(this->image);
		this->pressState->addElement(this->image);
		this->mouseOnState->addElement(this->image);
	}
	// Agregamos el label en todos los estados
	if(this->usesLabel()){
		this->basicState->addElement(this->label);
		this->pressState->addElement(this->label);
		this->mouseOnState->addElement(this->label);
	}
	this->internalState->makeStateTransition(this->basicState);	
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void UiButton::handleEvents(){
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == ONFOCUSHOLD_UIEVENTCODE) return;
		
	//Compruebo de que elemento se trata el evento y lo proceso
	if((this->onFocusEvent.getStateCode() == ONFOCUS_UIEVENTCODE)){
		this->processPressEvent();	
	}
	else if(this->onFocusEvent.getStateCode() == ONFOCUSRELEASE_UIEVENTCODE){
		this->processReleaseEvent();		
	}
	//Vacio el evento (No me interesa en este caso mantener estados de hold ni nada de eso)
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

void UiButton::processPressEvent(){
	this->internalState->makeStateTransition(this->pressState);
}
void UiButton::processMouseOnEvent(){
	this->internalState->makeStateTransition(this->mouseOnState);
}
void UiButton::processReleaseEvent(){
	this->internalState->makeStateTransition(this->basicState);
}

UiButton::~UiButton(){
	if(this->basicState != NULL){
		delete this->basicState;
		this->basicState = NULL;
	}
	if(this->pressState != NULL){
		delete this->pressState;
		this->pressState = NULL;
	}
	if(this->mouseOnState != NULL){
		delete this->mouseOnState;
		this->mouseOnState = NULL;
	}
	if(this->label != NULL){
		delete this->label;
		this->label = NULL;
	}
	if(this->image != NULL){
		delete this->image;
		this->image = NULL;
	}
}

bool UiButton::usesImage(){
	return (this->imgScale.x > 0.0f && this->imgScale.y > 0.0f);
}

bool UiButton::usesLabel(){
	return (strlen(this->font) > 0);
}
