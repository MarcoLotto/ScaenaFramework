/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andr�s Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIForm.h"
#include "UIStaticImage.h"
#include "UiButton.h"

#define POPUP_CLOSED_CALLBACK_ID 100

class TextInfoPopupForm : public UIForm{

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
	UIState* showedState;
	UIState* hidedState;

	UiLabel* textLine1;
	UiLabel* textLine2;
	UiLabel* textLine3;
	UiButton* button1;

	Callbackable* callbackElement;

	// Se definen los manejadores de eventos
	void processButton1Event();

public:
	TextInfoPopupForm(UIController* uiController);
	virtual ~TextInfoPopupForm();

	// Setea el texto de la caja de texto
	void setInfoText(string text);

	// Sacrifica una linea de texto pero permite poner un titulo a la caja de texto
	void setInfoTitle(string text);

	virtual void setVisible(bool value);

	// Define a quien decirle que termino cuando se cierre el popup
	void setCallbackElement(Callbackable* element);
};
