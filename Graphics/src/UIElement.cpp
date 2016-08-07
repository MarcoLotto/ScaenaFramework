/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UIElement.h"
#include "TimeManager.h"
#include "UIState.h"

UIElement::UIElement(UIController* uiController){
	//Guardo quien es el uiController
	this->uiController = uiController;
}

//Realiza la inicializacion del elemento llamando a la creacion de estados y elementos  
void UIElement::initiate(){
	//Inicializo los manejadores de estado
	this->internalState = new UIStateHolder();
	this->externalState = new UIStateHolder();
	//Inicializo sus estados default
	this->internalState->makeStateTransition(UIFactory::getBasicState());
	this->externalState->makeStateTransition(UIFactory::getBasicState());

	//Inicializo el evento nulo (no hay elementos en foco)
	this->onFocusEvent.setStateCode(NOEVENT_UIEVENTCODE);
	this->parentMergeAllowed = false;
		
	//Incializo los estados
	this->initiateStates();
	//Inicializo los sub elementos
	this->initiateElements();
	//Linkeo los elementos con los estados
	this->bindElementsToStates();
}

//Manejan el flujo de las transiciones de estados del elemento
void UIElement::stopTransitions(){
	//TODO: falta implementar.
}
void UIElement::startTransitions(){
	//TODO: falta implementar. Modificar el TimeManager	
}
void UIElement::pauseTransitions(){
	//TODO: falta implementar. Modificar el TimeManager
}

void UIElement::draw(){
	this->internalState->draw();
}

void UIElement::update(UIState* parentState){
	//Mergeo los estados
	UIState finalState = this->mergeParameters(parentState);

	//Update all
	this->internalState->update(&finalState);
	this->externalState->update(&finalState);
}

void UIElement::handleElementEvents(){
	// Actualizo todos los elementos hijos del estado actual
	list<UIElement*>* elements = this->internalState->getActualState()->getDrawingElements();
	list<UIElement*>::iterator it = elements->begin();
	while(it != elements->end()){
		(*it)->handleElementEvents();
		++it;
	}

	// Ahora actualizo al elemento en si
	this->handleEvents();
}

UIState UIElement::mergeParameters(UIState* parentState){
	//Si se permite el merge de escala del padre
	if(this->parentMergeAllowed)
		return this->externalState->getActualState()->mergeBasicParametersWithParentScale(parentState);
	//De lo contrario se procederá a un merge sin tener en cuenta escala
	return this->externalState->getActualState()->mergeBasicParametersWithoutParentScale(parentState);		
}

UIEventHandler UIElement::onClick(vec2 coords, int state){
	//Mergeo los estados
	UIState* externalState = this->getExternalState()->getActualState();
	
	//Guardo el evento que se produjo. Esto se revisará luego en handleEvents() y onKeyDown/Up
	this->onFocusEvent = this->internalState->evaluateClick(coords, state, externalState);

	//Devuelvo un codigo de foco por defecto, cambiar si es necesario en clases internas
	int stateCode = this->onFocusEvent.getStateCode();
	if(stateCode != NOEVENT_UIEVENTCODE){
		//Si el afectedElement es null, pero hay evento, me seteo a mi mismo
		if(this->onFocusEvent.getAfectedElement() == NULL){
			this->onFocusEvent.setAfectedElement(this);
		}
		//this->onFocusEvent.setCoords(coords);
		stateCode = ONFOCUS_UIEVENTCODE;
	}
	UIEventHandler eventHandler(this, stateCode, coords);
	return eventHandler;
}

//Adapta la coordenada para uniformizar rotaciones y translaciones
vec2 UIElement::getFixedCoordPoint(vec2 coords){
	//Consigo el merge entre el estado externo e interno
	UIState finalState = this->internalState->getActualState()->mergeBasicParameters(this->externalState->getActualState());
	//Calculo donde caen las coordenadas para el elemento
	mat4 rotMatForChilds = glm::translate(mat4(1.0f), vec3(-finalState.getPosition(), 0.0f));
	vec2 modifiedCoordsForChilds = vec2(rotMatForChilds * vec4(coords, 0.0f, 1.0f));
	rotMatForChilds = glm::rotate(mat4(1.0f), -finalState.getRotation(), vec3(0.0f, 0.0f, 1.0f));
	return vec2(rotMatForChilds * vec4(modifiedCoordsForChilds, 0.0f, 1.0f));	
}

UIEventHandler UIElement::onClickHold(vec2 coords, int state){	
	//Delego en el subelemento en foco. Cambiar en subClases
	if(this->onFocusEvent.getStateCode() != NOEVENT_UIEVENTCODE){
		this->onFocusEvent.setStateCode(ONFOCUSHOLD_UIEVENTCODE);
		UIElement* element = this->onFocusEvent.getAfectedElement();

		//Consigo la posicion del click en el hijo
		vec2 fixedCoords = this->getFixedCoordPoint(coords);
		if(element != this){
			//Envio al hijo con las coordenadas modificadas
			this->onFocusEvent = element->onClickHold(fixedCoords, state);
		}
		this->onFocusEvent.setCoords(fixedCoords);
	}
	UIEventHandler eventHandler(this, ONFOCUSHOLD_UIEVENTCODE, coords);
	return eventHandler;
}

UIEventHandler UIElement::onClickRelease(vec2 coords, int state){
	//Delego en el subelemento en foco. Cambiar en subClases
	if(this->onFocusEvent.getStateCode() != NOEVENT_UIEVENTCODE){
		this->onFocusEvent.setStateCode(ONFOCUSRELEASE_UIEVENTCODE);
		UIElement* element = this->onFocusEvent.getAfectedElement();
		//Consigo la posicion del click en el hijo
		vec2 fixedCoords = this->getFixedCoordPoint(coords);
		if(element != this){
			//Envio al hijo con las coordenadas modificadas
			this->onFocusEvent = element->onClickRelease(coords, state);
		}
		this->onFocusEvent.setCoords(fixedCoords);
	}	
	UIEventHandler eventHandler(this, ONFOCUSRELEASE_UIEVENTCODE, coords);
	return eventHandler;
}

void UIElement::onKeyDown(int key){
	//Delego en el subelemento en foco. Cambiar en subClases.Devuelvo codigo de foco por defecto
	if(this->onFocusEvent.getStateCode() != NOEVENT_UIEVENTCODE){
		UIElement* element = this->onFocusEvent.getAfectedElement();
		if(element != this){
			element->onKeyDown(key);
		}		
	}
}
void UIElement::onKeyUp(int key){
	//Delego en el subelemento en foco. Cambiar en subClases
	if(this->onFocusEvent.getStateCode() != NOEVENT_UIEVENTCODE){
		UIElement* element = this->onFocusEvent.getAfectedElement();
		if(element != this){
			element->onKeyUp(key);
		}
	}
}

void UIElement::deleteElement(UIElement* element){
	if(element != NULL){
		delete element;
		element = NULL;
	}
}
void UIElement::deleteState(UIState* state){
	if(state != NULL){
		delete state;
		state = NULL;
	}
}

UIElement::~UIElement(){
	if(this->externalState != NULL){
		delete this->externalState;
		this->externalState = NULL;
	}
	if(this->internalState != NULL){
		delete this->internalState;
		this->internalState = NULL;
	}
}
