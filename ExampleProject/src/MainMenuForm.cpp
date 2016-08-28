/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#define BARS_OPENED_EVENT 10

#include "MainMenuForm.h"

MainMenuForm::MainMenuForm(UIController* uiController) : UIForm(uiController){
	this->initiate();	
}

//Incializacion de los estados del elemento 
void MainMenuForm::initiateStates(){
	this->openState = UIFactory::createState(vec2(0.0f, 0.0f), 0.0f, vec2(0.265f, 0.8f), 1.0f);
	this->closeState = UIFactory::createState(vec2(0.0f, 0.0f), 0.0f, vec2(0.265f, 0.8f), 1.0f);

	float hidenBarHeight = 0.1f;
	this->bar1ShowedState = UIFactory::createState(vec2(0.0f, 0.0f), 0.0f, vec2(0.095f, 0.8f), 0.8f);
	this->bar1HidenState = UIFactory::createState(vec2(0.0f, 0.0f), 0.0f, vec2(0.095f, hidenBarHeight), 0.5f);

	this->bar2ShowedState = UIFactory::createState(vec2(0.098f, 0.0f), 0.0f, vec2(0.08f, 0.7f), 0.7f);
	this->bar2HidenState = UIFactory::createState(vec2(0.098f, 0.0f), 0.0f, vec2(0.08f, hidenBarHeight), 0.5f);

	this->bar3ShowedState = UIFactory::createState(vec2(0.18f, 0.0f), 0.0f, vec2(0.065f, 0.6f), 0.6f);
	this->bar3HidenState = UIFactory::createState(vec2(0.18f, 0.0f), 0.0f, vec2(0.065f, hidenBarHeight), 0.5f);

}
//Inicializacion de los subelementos
void MainMenuForm::initiateElements(){
	this->sceneButton1 = UIFactory::createButton(vec2(0.01f, 0.1f), 0.0f, vec2(0.07f, 0.1f), "", 0.1f, "./Textures/UI/button1.png", this->uiController);
	this->sceneButton2 = UIFactory::createButton(vec2(0.01f, 0.25f), 0.0f, vec2(0.07f, 0.1f), "", 0.1f, "./Textures/UI/button1.png", this->uiController);
	this->optionsButton = UIFactory::createButton(vec2(0.01f, 0.4f), 0.0f, vec2(0.07f, 0.1f), "", 0.1f, "./Textures/UI/button1.png", this->uiController);
	this->sceneNameLabel1 = UIFactory::createLabel("Scene1", vec2(0.0f), 0.1f, "./Textures/UI/font2.png", this->uiController);
	this->sceneNameLabel2 = UIFactory::createLabel("Scene2", vec2(0.0f), 0.1f, "./Textures/UI/font2.png", this->uiController);

	this->bar1 = UIFactory::createStaticImage(vec2(0.0f, 0.0f), 0.0f, vec2(0.095f, 0.8f), 0.5f, "./Textures/UI/bar1.png", this->uiController);
	this->bar2 = UIFactory::createStaticImage(vec2(0.098f, 0.0f), 0.0f, vec2(0.08f, 0.7f), 0.5f, "./Textures/UI/bar1.png", this->uiController);
	this->bar3 = UIFactory::createStaticImage(vec2(0.18f, 0.0f), 0.0f, vec2(0.065f, 0.6f), 0.5f, "./Textures/UI/bar1.png", this->uiController);
}

void MainMenuForm::bindElementsToStates(){
	// Asigno a cada estado los elementos que contendran
	this->openState->addElement(this->bar1);	
	this->openState->addElement(this->bar2);	
	this->openState->addElement(this->bar3);	
	this->openState->addElement(this->sceneButton1);
	this->openState->addElement(this->sceneButton2);
	this->openState->addElement(this->optionsButton);
	//this->openState->addElement(this->sceneNameLabel1);
	//this->openState->addElement(this->sceneNameLabel2);	

	this->closeState->addElement(this->bar1);	
	this->closeState->addElement(this->bar2);	
	this->closeState->addElement(this->bar3);
	
	//Inicializo el estado inicial
	this->internalState->makeStateTransition(this->openState);	
}

void MainMenuForm::processScene1ButtonClickEvent(){

}
void MainMenuForm::processScene2ButtonClickEvent(){

}
void MainMenuForm::processOptionsButtonClickEvent(){

}
void MainMenuForm::processEventMenuClickEvent(){
	if(this->onFocusEvent.getStateCode() == ONFOCUS_UIEVENTCODE){
		if(this->internalState->isTheFinalState(this->openState)){
			// Cambio de estado para esconder los botones
			this->internalState->makeStateTransition(this->closeState);

			// Levanto las barras
			this->bar1->getExternalState()->makeStateTransition(this->bar1HidenState, 2000);	
			this->bar2->getExternalState()->makeStateTransition(this->bar2HidenState, 2000);	
			this->bar3->getExternalState()->makeStateTransition(this->bar3HidenState, 2000);	
		}
		else if(this->internalState->isTheFinalState(this->closeState)){
			// Cambio de estado para mostrar los botones despues de un tiempo
			this->internalState->makeStateTransition(this->openState, 2000);
			
			// Despues levanto las barras
			this->bar1->getExternalState()->makeStateTransition(this->bar1ShowedState, 2000);	
			this->bar2->getExternalState()->makeStateTransition(this->bar2ShowedState, 2000);	
			this->bar3->getExternalState()->makeStateTransition(this->bar3ShowedState, 2000);	
		}
	}
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void MainMenuForm::handleEvents(){
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;

	UIElement* afectedElement = this->onFocusEvent.getAfectedElement();
	
	//Compruebo de que elemento se trata el evento y lo proceso
	if(afectedElement == this->sceneButton1){
		this->processScene1ButtonClickEvent();	
	}
	if(afectedElement == this->sceneButton2){
		this->processScene2ButtonClickEvent();	
	}	
	if(afectedElement == this->optionsButton){
		this->processOptionsButtonClickEvent();	
	}	
	if(afectedElement == this->sceneButton1 || afectedElement == this->sceneButton2 || afectedElement == this->optionsButton){
		this->processEventMenuClickEvent();	
	}
	if(afectedElement == this->bar1 || afectedElement == this->bar2 || afectedElement == this->bar3){
		this->processEventMenuClickEvent();	
	}	
	
	//Vacio el evento (No me interesa en este caso mantener estados de hold ni nada de eso)
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

MainMenuForm::~MainMenuForm(){
	delete this->openState;
	delete this->closeState;

	delete this->sceneButton1;
	delete this->sceneButton2;
	delete this->optionsButton;
	delete this->sceneNameLabel1;
	delete this->sceneNameLabel2;
	delete this->bar1;
	delete this->bar2;
	delete this->bar3;
}