/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIForm.h"
#include "UiButton.h"
#include "UITextBox.h"
#include "UIScrollBar.h"
#include "UIList.h"

class TestForm1 : public UIForm{

private:
	//Incializacion de los estados del elemento 
	void initiateStates();
	//Inicializacion de los subelementos
	void initiateElements();
	//Linkea los elementos con los estados (cuales deben ser dibujados en cada estado)
	void bindElementsToStates();

	//Estados
	UIState* basicState;
	UIState* transformedState;
	UIState* dynamicState;
	UIState* holdingState;
	
	//Agrego los subelementos
	UiButton* button1;
	UITextBox* textBox1;
	UiLabel* label1;
	UiLabel* label2;

	
	//Auxiliares para procesamientos de eventos
	void processEvent1();
	void processEvent2();

public:
	TestForm1(UIController* uiController);
	virtual ~TestForm1();

	//Maneja los eventos que se producen cuando un elemento consigue el foco
	void handleEvents();
};