/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIElement.h"

class UICheckBox : public UIElement{

protected:
	//Maneja los eventos que se producen cuando un elemento consigue el foco
	void handleEvents();

private:
	//Incializacion de los estados del elemento 
	void initiateStates();
	//Inicializacion de los subelementos
	void initiateElements();
	//Linkea los elementos con los estados (cuales deben ser dibujados en cada estado)
	void bindElementsToStates();

	//Estados
	UIState* uncheckedState;
	UIState* pressState;
	UIState* checkedState;

	//Otros atributos
	const char* filename;
	bool checked;
	
	//Auxiliares para procesamientos de eventos
	void processPressEvent();
	void processReleaseEvent();

public:
	UICheckBox(const char* filename, UIController* uiController);
	virtual ~UICheckBox();

	void setChecked(bool value);
	bool isChecked(){ return this->checked; }

};