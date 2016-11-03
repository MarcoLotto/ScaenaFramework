/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UIDynamicImage.h"

UIDynamicImage::UIDynamicImage(const char* filename, UIController* uiController, int animationRows, int animationTime, bool loop) : UIElement(uiController){
	//La ruta de la imagen a cargar
	this->filename = filename;
	this->animationRows = animationRows;
	this->animationTime = animationTime;
	this->loop = loop;

	//Como no es un form, tengo que inicializar a mano
	this->initiate();
}

//Incializacion de los estados del elemento 
void UIDynamicImage::initiateStates(){
	float divisions = 1.0f / (float) this->animationRows;
	this->state = UIFactory::createDynamicTextureState(vec2(0.0f), 0.0f, vec2(1.0f, 1.0f), 1.0f, this->filename, vec2(divisions), vec2(0.0f), this->animationRows, this->animationTime, this->loop);	
}

//Inicializacion de los subelementos
void UIDynamicImage::initiateElements(){
	// No tengo elementos
}

void UIDynamicImage::bindElementsToStates(){
	// Solo inicializo el estado inicial
	this->internalState->makeStateTransition(this->state);	
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void UIDynamicImage::handleEvents(){
	// No me interesa realizar acciones especiales dentro del elemento
}

UIDynamicImage::~UIDynamicImage(){
	if(this->state != NULL){
		delete this->state;
		this->state = NULL;
	}	
}

void UIDynamicImage::setTexture(Texture* texture){
	this->state->setTexture(texture);
	this->internalState->makeStateTransition(this->state);
}