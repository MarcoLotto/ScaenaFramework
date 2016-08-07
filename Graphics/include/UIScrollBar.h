/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIElement.h"
#include "UiButton.h"

class UIScrollBar : public UIElement{

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
	UIState* basicState;

	//Elementos
	UiButton* upButton;
	UiButton* downButton;
	UiButton* moveButton;

	//Otros atributos
	const char* filename;
	float scrollFactor;
	float scrollFactorIncrement;
	float scrollBarTop;
	float motionDiference;

	//Auxiliares para procesamientos de eventos
	void processUpButtonEvent();
	void processDownButtonEvent();
	void processMoveButtonEvent();

	//Otros metodos privados
	float calculateFactorFromPosition(vec2 coords);

public:
	UIScrollBar(const char* filename, UIController* uiController);
	virtual ~UIScrollBar();

	//Setea el ancho de TODO el control
	void setWidth(float value);
	//Setea el largo de la barra
	void setHeight(float value);

	//Accesors para el factor de scroll, este va entre 0 y 1 y determina en que punto de scroll se encuentra
	void setScrollFactor(float factor);
	float getScrollFactor(){ return this->scrollFactor; }

	void setScrollFactorIncrement(float value){ this->scrollFactorIncrement = value; }
	float getScrollFactorIncrement(){ return this->scrollFactorIncrement; }
};