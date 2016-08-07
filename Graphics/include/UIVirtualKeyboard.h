/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UiButton.h"
#include <string>
using namespace std;

class UIVirtualKeyboard : public UIElement{

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

	void presentKeyboard(char* diccionary);
	void buildDiccionary();

	bool isLowerCaseActive, isUpperCaseActive, lastKeyWasEnter;
	UIElement* focusedElement;
	string normalButtonsBackground, specialButtonsBackground;
	
	//Estados	
	UIState* showedState; 

	// Elementos
	vector<UiButton*> buttons;
	UiButton* shiftButton;
	UiButton* spaceButton;
	UiButton* backspaceButton;
	UiButton* enterButton;
	UiButton* modeChangeButton;

	// Handlers
	void processButtonEvent(int i);
	void processShiftEvent();
	void processSpaceEvent();
	void processBackspaceEvent();
	void processEnterEvent();
	void processModeEvent();

public:
	UIVirtualKeyboard(string normalButtonsBackground, string specialButtonsBackground, UIController* uiController);
	virtual ~UIVirtualKeyboard();

	// Por si se quiere forzar a presentar algun teclado en particular
	void presentLowerCaseKeyboard();
	void presentUpperCaseKeyboard();
	void presentSymbolKeyboard();
	
	// Identifica cual es el elemento que tiene foco a la hora de utilizar el virtualKeyboard
	void setFocusedElement(UIElement* element){ this->focusedElement = element; }

	// Identifica si la ultima tecla presionada fue un enter
	bool isLastKeyEnter(){ return this->lastKeyWasEnter; }
};