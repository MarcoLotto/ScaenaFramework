/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIForm.h"
#include "UIController.h"

class FormsManager{

public:
	//**********Declaracion de forms***************
	UIForm* backgroundForm;
	UIForm* menuStatusBarForm;
	UIForm* virtualKeyboardForm;
	UIForm* textInfoPopupForm;
	UIForm* shirtsTeamBuilderForm;
	UIForm* pantsTeamBuilderForm;
	UIForm* socksTeamBuilderForm;
	UIForm* teamBuilderDataForm;
	//*********************************************

private:
	UIController* uiController;
	static FormsManager* instance;	

	void initAndAddFormToUIController(UIForm* form, vec2 pos, float rot, vec2 esc, bool visible, bool goingToFrontOnFocus=false);

	//Borra toda la escena actual para poder cargar otra
	void resetAllScene();
	
public:
	static FormsManager* getInstance();
	static FormsManager* getInstance(UIController* uiController);
	FormsManager();

	// Acá se declaran todas las forms que se utilizarán dentro del menu
	void initializeMenuForms();

	void setUIController(UIController* controller){ this->uiController = controller; }
	UIController* getUIController(){ return this->uiController; }
};