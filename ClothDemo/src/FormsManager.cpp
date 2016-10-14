/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "FormsManager.h"
#include "PropertyCannotBeAcessedException.h"
#include "ProgramParameters.h"

#include "MenuStatusBarForm.h"
#include "TeamBuilderForm.h"
#include "TeamBuilderDataForm.h"
#include "VirtualKeyboardForm.h"
#include "TextInfoPopupForm.h"

FormsManager* FormsManager::instance = NULL;

FormsManager* FormsManager::getInstance(){
	if(instance == NULL){
		throw new PropertyCannotBeAcessedException("FormManager necesita que se llame primero a getInstance con el UIController como parametro");
	}
	return instance;
}
FormsManager* FormsManager::getInstance(UIController* uiController){
	if(instance == NULL){
		instance = new FormsManager();
		instance->setUIController(uiController);
	}	
	return instance;
}

FormsManager::FormsManager(){
	this->uiController = NULL;
}

// Acá se declaran todas las forms que se utilizarán dentro del menu
void FormsManager::initializeMenuForms(){	
	//Etapa de instanciacion***************	
	this->menuStatusBarForm = new MenuStatusBarForm(this->uiController);
	this->textInfoPopupForm = new TextInfoPopupForm(this->uiController);
	this->socksTeamBuilderForm = new TeamBuilderForm(this->uiController, SOCKS_BUILDER_CONFIG_FILENAME, NULL);
	this->pantsTeamBuilderForm = new TeamBuilderForm(this->uiController, PANTS_BUILDER_CONFIG_FILENAME, (TeamBuilderForm*) this->socksTeamBuilderForm);
	this->shirtsTeamBuilderForm = new TeamBuilderForm(this->uiController, SHIRTS_BUILDER_CONFIG_FILENAME, (TeamBuilderForm*) this->pantsTeamBuilderForm);
	this->teamBuilderDataForm = new TeamBuilderDataForm(this->uiController);
	this->virtualKeyboardForm = new VirtualKeyboardForm(this->uiController);	

	//Etapa de inicializacion***************	
	this->initAndAddFormToUIController(this->shirtsTeamBuilderForm, vec2(0.0f), 0.0f, vec2(1.0f), false);
	this->initAndAddFormToUIController(this->pantsTeamBuilderForm, vec2(0.0f), 0.0f, vec2(1.0f), false);
	this->initAndAddFormToUIController(this->socksTeamBuilderForm, vec2(0.0f), 0.0f, vec2(1.0f), false);
	this->initAndAddFormToUIController(this->teamBuilderDataForm, vec2(0.0f), 0.0f, vec2(1.0f), false);
	this->initAndAddFormToUIController(this->menuStatusBarForm, vec2(0.0f), 0.0f, vec2(1.0f), false);
	this->initAndAddFormToUIController(this->virtualKeyboardForm, vec2(0.0f), 0.0f, vec2(1.0f), false);
	this->initAndAddFormToUIController(this->textInfoPopupForm, vec2(0.0f), 0.0f, vec2(1.0f), false);
}

void FormsManager::initAndAddFormToUIController(UIForm* form, vec2 pos, float rot, vec2 esc, bool visible, bool goingToFrontOnFocus){
	//Lo inicializo	
	form->setVisible(visible);

	//Asigno un estado externo
	UIState* state = UIFactory::createState(pos, rot, esc, 1.0f, NULL);
	form->getExternalState()->makeStateTransition(state);

	//Agrego el form
	this->uiController->addForm(form);

	// Asigno si va o no al frente al ganar foco
	form->setGoingToFrontOnFocus(goingToFrontOnFocus);
}

void FormsManager::resetAllScene(){
	
}
