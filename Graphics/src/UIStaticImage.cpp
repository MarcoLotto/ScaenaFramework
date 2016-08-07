/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UIStaticImage.h"

UIStaticImage::UIStaticImage(const char* filename, UIController* uiController) : UIElement(uiController){
	//La ruta de la imagen a cargar
	this->filename = filename;

	//Como no es un form, tengo que inicializar a mano
	this->initiate();
}

//Incializacion de los estados del elemento 
void UIStaticImage::initiateStates(){
	this->basicState = UIFactory::createState(vec2(0.0f), 0.0f, vec2(1.0f, 1.0f), 1.0f, this->filename);	
}

//Inicializacion de los subelementos
void UIStaticImage::initiateElements(){
	// No tengo elementos
}

void UIStaticImage::bindElementsToStates(){
	// Solo inicializo el estado inicial
	this->internalState->makeStateTransition(this->basicState);	
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void UIStaticImage::handleEvents(){
	// No me interesa realizar acciones especiales dentro del elemento
}

UIStaticImage::~UIStaticImage(){
	if(this->basicState != NULL){
		delete this->basicState;
		this->basicState = NULL;
	}	
}

void UIStaticImage::setTexture(Texture* texture){
	this->basicState->setTextureAndDeletePrevious(texture);
	this->internalState->makeStateTransition(this->basicState);	
}