/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

class UIElement;

//**************************************************************************
//* Clase para identificar eventos de interfaz de usuario                  *
//**************************************************************************

//Identifica el codigo para identificar que no hay eventos
#define WAITING_UIEVENTCODE -1
#define NOEVENT_UIEVENTCODE 0

#define ONFOCUS_UIEVENTCODE 1
#define ONFOCUSHOLD_UIEVENTCODE 2
#define ONFOCUSRELEASE_UIEVENTCODE 3

#define ONCLOSE_UIEVENTCODE 4
#define ONMOVE_UIEVENTCODE 5

class UIEventHandler{

private:
	UIElement* afectedElement;
	int stateCode;
	vec2 coords;

public:
	UIEventHandler(){
		this->afectedElement = NULL;
		this->stateCode = NOEVENT_UIEVENTCODE;
		this->coords = vec2(0.0f);
	}
	UIEventHandler(UIElement* element, int stateCode, vec2 coords){
		this->afectedElement = element;
		this->stateCode = stateCode;
		this->coords = coords;
	}

	void setAfectedElement(UIElement* element){ this->afectedElement = element; }
	UIElement* getAfectedElement(){ return this->afectedElement; }

	void setCoords(vec2 coords){ this->coords = coords; }
	vec2 getCoords(){ return this->coords; }
	
	void setStateCode(int code){ this->stateCode = code;}
	int getStateCode(){return this->stateCode;}
};