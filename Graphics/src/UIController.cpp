/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UIController.h"

UIController::UIController(){
	this->emptyState = new UIState();	
	this->holdingClick = false;
	this->holdingClickState = 0;
    this->pointsToPixelsScaleFactor = vec2(1.0f);
	this->formOnFocus = NULL;
}

void UIController::draw(){
	//Dibujo cada elemento (siempre y cuando este actualizado)
	std::list<UIForm*>::iterator it = this->forms.begin();
	while(it != this->forms.end()){
		if((*it)->isVisible())
			(*it)->draw();
		++it;
	}
}
void UIController::update(){
	// Primero verifico si hay forms para agregar o dar foco
	this->updateFocusedFormsAndNewForms();

	// Actualizo los eventos de todos los forms (tiene que hacerse antes del update)
	std::list<UIForm*>::iterator itEvents = this->forms.begin();
	while(itEvents != this->forms.end()){
		if((*itEvents)->isVisible())
			(*itEvents)->handleFormEvents();				
		++itEvents;
	}
	// Update de cada form
	std::list<UIForm*>::iterator itUpdate = this->forms.begin();
	while(itUpdate != this->forms.end()){
		if((*itUpdate)->isVisible())
			(*itUpdate)->update(this->emptyState);				
		++itUpdate;
	}
	// Verifico los holding del mouse (luego del update)
	this->manageHoldedClick();
}

vec2 UIController::normalizeCoords(vec2 coords){
	return (this->pointsToPixelsScaleFactor * coords) / this->screenSize;
}

void UIController::manageHoldedClick(){
	//Compruebo si se estaba sosteniendo el click
	if(this->holdingClick){
		if(this->onFocusEvent.getStateCode() != NOEVENT_UIEVENTCODE){
			UIForm* form = (UIForm*) this->onFocusEvent.getAfectedElement();
			if(form->isVisible())
				form->onClickHold(this->mouseActualCoords, this->holdingClickState);
		}
	}
}

UIForm* UIController::reorganizeForms(std::list<UIForm*>::reverse_iterator it){
	//Saco al elemento de la lista para ponerlo al frente
	this->formOnFocus = (*it);
	if(this->formOnFocus->isGoingToFrontOnFocus())
		this->forms.erase(--(it).base());  //Manera extraña para borrar reverse_iterators mas o menos eficiente
	else
		return NULL;
	return this->formOnFocus;
}

void UIController::manageClick(vec2 coords, int state){
	//Normalizo las coordenadas entre 0 y 1
	coords = this->normalizeCoords(coords);
	
	UIForm* clickedForm = NULL;
	//Compruebo a quien le pertenece el click cada elemento
	std::list<UIForm*>::reverse_iterator it = this->forms.rbegin();
	while(it != this->forms.rend()){
		if((*it)->isVisible()){
			//Seteo el nuevo elemento en foco
			this->onFocusEvent = (*it)->onClick(coords, state);
			if(this->onFocusEvent.getStateCode() != NOEVENT_UIEVENTCODE){
				//Encontre un click en un elemento
				this->holdingClick = true;
				this->holdingClickState = state;
				this->mouseActualCoords = coords;

				// Le avisamos al form en foco anterior que perdio el foco
				if(this->formOnFocus != (*it) && this->formOnFocus != NULL){
					this->formOnFocus->onLostFocus();
				}
				//Si es necesario borro el form de la lista de forms activos
				clickedForm = this->reorganizeForms(it);			
				break;
			}
		}
		++it;
	}
	//Si es necesario agrego el form para que se vea al frente
	if(clickedForm != NULL){
		this->forms.push_back(clickedForm);
	}
}

void UIController::manageClickRelease(vec2 coords, int state){
	//Normalizo las coordenadas entre 0 y 1
	coords = this->normalizeCoords(coords);

	if(this->onFocusEvent.getStateCode() != NOEVENT_UIEVENTCODE){
		UIForm* form = (UIForm*) this->onFocusEvent.getAfectedElement();
		if(form->isVisible())
			form->onClickRelease(coords, state);
		this->holdingClick = false;
	}
}

void UIController::manageKeyDown(int key){
	if(this->onFocusEvent.getStateCode() != NOEVENT_UIEVENTCODE){
		this->onFocusEvent.getAfectedElement()->onKeyDown(key);
	}
}
void UIController::manageKeyUp(int key){
	if(this->onFocusEvent.getStateCode() != NOEVENT_UIEVENTCODE){
		this->onFocusEvent.getAfectedElement()->onKeyUp(key);
	}
}

void UIController::addForm(UIForm* form){ 
	this->giveFocusTo(form);
}

void UIController::setMouseActualCoords(vec2 absoluteCoords){
	//Guardo las coordenadas del mouse normalizadas
	this->mouseActualCoords = normalizeCoords(absoluteCoords);
}

// Agrega para renderizar y actualiza aquellos forms que tomaron foco o se agregaron durante el ultimo frame
void UIController::updateFocusedFormsAndNewForms(){
	std::list<UIForm*>::iterator it = this->formsToUpdate.begin();
	while(it != this->formsToUpdate.end()){
		// Mando a reorganizar/agregar el form
		this->searchAndReorganizeForm(*it);
		it = this->formsToUpdate.erase(it);
	}	
}

void UIController::searchAndReorganizeForm(UIForm* form){
	bool addToFormList = true;
	//Busco el form entre los existentes, si lo encuentro primero lo borro de la lista y lo hago visible
	std::list<UIForm*>::reverse_iterator it = this->forms.rbegin();
	while(it != this->forms.rend()){
		if((*it) == form){
			if(this->reorganizeForms(it) == NULL){
				addToFormList = false;
			}
			form->setVisible(true);
			break;
		}
		++it;
	}
	// Ya no esta mas el form en la lista de forms (si alguna vez estuvo), lo agrego al final para que se renderice sobre los demas forms
	if(addToFormList){
		this->forms.push_back(form);
	}
}

//Le da foco a un form en particular. Si el form no existe lo agrega.
void UIController::giveFocusTo(UIForm* form){
	//Le seteo el uiController
	form->setUiController(this);
	//Agrega el form como pendiente para dar foco/agregar
	this->formsToUpdate.push_back(form);
}


