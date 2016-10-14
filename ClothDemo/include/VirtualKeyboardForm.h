#pragma once
#include "UIForm.h"
#include "UIStaticImage.h"
/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UIVirtualKeyboard.h"

class VirtualKeyboardForm : public UIForm{

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

	void processKeyboardEvent();
	
	//Estados
	UIState* showedState;
	UIState* hidedState;
    bool inStateTransition;
		
	//Agrego los subelementos
	UIVirtualKeyboard* keyboard;

public:
	VirtualKeyboardForm(UIController* uiController);
	virtual ~VirtualKeyboardForm();

	virtual void setVisible(bool value);

	void onCallback(int callbackId);

	// Identifica cual es el elemento que tiene foco a la hora de utilizar el virtualKeyboard
	void setFocusedElement(UIElement* element){ this->keyboard->setFocusedElement(element); }

	// Se ejecuta cuando el form pierde foco
	virtual void onLostFocus();
};