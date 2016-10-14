/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIForm.h"
#include "UITextBox.h"
#include "UICheckBox.h"
#include "TeamBuilderDataTransfer.h"

class TeamBuilderDataForm : public UIForm{

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

	void onScreenChange();
	bool runFieldValidations();

	//Estados
	UIState* showedState;
	UIState* hidedState;
		
	//Agrego los subelementos
	UITextBox* input;
	UiButton* continueButton;
	UICheckBox* uiCheckbox;
	UiLabel* checkboxLabel;

	// Atributos propios del form
	TeamBuilderDataTransfer dataTransfer;

	// Se definen los manejadores de eventos
	void processInputEvent();
	void processContinueButtonEvent();
	
public:
	TeamBuilderDataForm(UIController* uiController);
	virtual ~TeamBuilderDataForm();

	void setVisible(bool value);
	void onCallback(int callbackId);

	// Usado para pasar datos de un builder a otro
	void setTeamBuilderDataTransfer(TeamBuilderDataTransfer dataTransfer){ this->dataTransfer = dataTransfer; }
};