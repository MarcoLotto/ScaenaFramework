/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIElement.h"
#include "UIFactory.h"

class UIForm : public UIElement{

private:
	bool goingToFrontOnFocus;

protected:
	bool visible;

public:
	UIForm(UIController* uiController);

	// Identifica si el form pasa adelante de los demas cuando gana foco
	bool isGoingToFrontOnFocus(){ return this->goingToFrontOnFocus; }
	void setGoingToFrontOnFocus(bool value){ this->goingToFrontOnFocus = value; }

	// Identifica si el form es visible
	bool isVisible(){ return (this->visible && this->internalState->isVisible() && this->externalState->isVisible()); }
	virtual void setVisible(bool value){ this->visible = value;	}

	// Manda a manejar todos los eventos que se hayan producido en este form o sus elementos
	void handleFormEvents();

	// Se ejecuta cuando el form pierde foco
	virtual void onLostFocus(){}
};