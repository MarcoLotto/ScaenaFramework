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
#include "UIStaticImage.h"

class MainMenuForm : public UIForm{

private:
	//Incializacion de los estados del elemento 
	void initiateStates();
	//Inicializacion de los subelementos
	void initiateElements();
	//Linkea los elementos con los estados (cuales deben ser dibujados en cada estado)
	void bindElementsToStates();

	//Estados
	UIState* openState;
	UIState* closeState;
	UIState* bar1ShowedState;
	UIState* bar1HidenState;
	UIState* bar2ShowedState;
	UIState* bar2HidenState;
	UIState* bar3ShowedState;
	UIState* bar3HidenState;
	
	//Agrego los subelementos
	UiButton* sceneButton1;
	UiButton* sceneButton2;
	UiButton* optionsButton;
	UiLabel* sceneNameLabel1;
	UiLabel* sceneNameLabel2;
	UIStaticImage* bar1;
	UIStaticImage* bar2;
	UIStaticImage* bar3;
	
	//Auxiliares para procesamientos de eventos
	void processScene1ButtonClickEvent();
	void processScene2ButtonClickEvent();
	void processOptionsButtonClickEvent();
	void processEventMenuClickEvent();

public:
	MainMenuForm(UIController* uiController);
	virtual ~MainMenuForm();

	//Maneja los eventos que se producen cuando un elemento consigue el foco
	void handleEvents();
};