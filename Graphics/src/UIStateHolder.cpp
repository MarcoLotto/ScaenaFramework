/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UIStateHolder.h"
#include "TimeManager.h"
#include "UIElement.h"

UIStateHolder::UIStateHolder(){
	//Inicializo el time slot
	this->timeSlot = TimeManager::getInstance()->reservTimeSlot();
	//Inicializo estados
	this->initialState = NULL;
	this->actualState = NULL;
	this->finalState = NULL;
	this->transitionEnabled = false;
	this->updated = false;
	this->callbackElement = NULL;
	this->callbackId = -1;
}

//Interpola entre los estados para calcular el estado actual
void UIStateHolder::interpolateStates(){
	if(!this->transitionEnabled) return;

	TimeManager* timeManager = TimeManager::getInstance();
	int elapsedTime = timeManager->getElapsedTime(this->timeSlot);
	float interval = elapsedTime / ((float)this->transitionTime + 1.0f);

	//Condicion de corte
	if(interval >= 1.0f){
		// Si hay, ejecuto la funcion de finalizado
		if(this->callbackElement != NULL && this->callbackId >= 0){
			this->callbackElement->onCallback(this->callbackId);
		}		
		//Si tengo animacion en loop, vuelvo al principio
		this->transitionEnabled = false;
		if(this->loopTransition){
			this->transitionEnabled = true;
			timeManager->updateTimeSlot(this->timeSlot);
		}
		else{
			//El estado actual tiene que ser igual al estado final
			this->makeStateTransition(this->finalState);
		}		
	}
	//Interpolo
	this->initialState->interpolate(this->finalState, this->actualState, interval);
}

//Realiza la interpolacion entre estados, y actualiza el elemento
void UIStateHolder::update(UIState* parentState){
	//Consigo el estado actual
	this->interpolateStates();

	//Actualizo el elemento
	this->actualState->update(parentState);

	//Marco al estado como actualizado
	this->updated = true;
}

//Va de un estado inicial a un estado final en un tiempo determinado
void UIStateHolder::makeStateTransition(UIState* startState, UIState* endState, int time, bool loop){
	if(time < 0) return;
	
	if(this->actualState != NULL){
		delete this->actualState;
	}
	if((this->initialState != NULL)&&(this->initialState->isDestroyedAfterTransition())){
		delete this->initialState;
	}
	this->initialState = startState;
	this->actualState = startState->clone();
	this->finalState = endState;

	this->transitionTime = time;
	TimeManager::getInstance()->updateTimeSlot(this->timeSlot);
	this->transitionEnabled = true;
	this->loopTransition = loop;

	this->callbackElement = NULL;
	this->callbackId = -1;

	//Marco al estado como desactualizado
	this->updated = false;
}

void UIStateHolder::makeStateTransition(UIState* startState, UIState* endState, int time, bool loop, UIElement* callbackElement, int callbackId ){	
	this->makeStateTransition(startState, endState, time, loop);
	this->callbackElement = callbackElement;
	this->callbackId = callbackId;
}

//Va del estado actual a un estado final en un tiempo determinado
void UIStateHolder::makeStateTransition(UIState* endState, int time, bool loop){
	if((time < 0)||(this->actualState == NULL)) return;

	if((this->initialState != NULL)&&(this->initialState->isDestroyedAfterTransition() && (this->initialState != this->actualState->getReferenceToOriginal()))){
		delete this->initialState;
	}	
	this->initialState = this->actualState->getReferenceToOriginal();
	this->finalState = endState;

	this->transitionTime = time;	
	TimeManager::getInstance()->updateTimeSlot(this->timeSlot);
	this->transitionEnabled = true;
	this->loopTransition = loop;

	this->callbackElement = NULL;
	this->callbackId = -1;

	//Marco al estado como desactualizado
	this->updated = false;
}

void UIStateHolder::makeStateTransition(UIState* endState, int time, bool loop, UIElement* callbackElement, int callbackId){
	this->makeStateTransition(endState, time, loop);
	this->callbackElement = callbackElement;
	this->callbackId = callbackId;
}

//Va a un estado final instantaneamente
void UIStateHolder::makeStateTransition(UIState* endState){
	if(this->actualState != NULL){
		delete this->actualState;
	}
	if((this->initialState != NULL)&&(this->initialState->isDestroyedAfterTransition())){
		delete this->initialState;
	}
	this->initialState = endState;
	this->actualState = endState->clone();
	this->finalState = endState;
	this->transitionEnabled = false;
	this->loopTransition = false;

	this->callbackElement = NULL;
	this->callbackId = -1;

	//Marco al estado como desactualizado
	this->updated = false;
}

void UIStateHolder::draw(){
	// Si el estado esta actualizado, dibujo
	if(this->updated)
		this->actualState->draw();
}

//Evalua si se realizo click en algun elemento de este estado
UIEventHandler UIStateHolder::evaluateClick(vec2 coords, int state, UIState* parentState){
	return this->actualState->evaluateClick(coords, state, parentState);
}	

UIStateHolder::~UIStateHolder(){
	//Solo borro el estado actual, los otros toman las instancias de states de los UIElements
	if(this->actualState != NULL){
		delete this->actualState;
		this->actualState = NULL;
	}	
}

bool UIStateHolder::isVisible(){
	return (this->actualState != NULL && this->actualState->getTransparency() > 0.0f) || (this->finalState != NULL && this->finalState->getTransparency() > 0.0f);
}