/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "MenuStatusBarForm.h"
#include "Logger.h"
#include "FormsManager.h"

MenuStatusBarForm::MenuStatusBarForm(UIController* uiController) : UIForm(uiController){	
	this->actualForm = NULL;
	this->initiate();	
	this->updateBarDisplayData();
}

//Incializacion de los estados del elemento 
void MenuStatusBarForm::initiateStates(){		
	this->state1 = UIFactory::createState(vec2(-0.05f, 0.0f), 0.0f, vec2(1.1f, 0.1f), 1.0f, "Textures/UI/FormBackground1.png");
    this->noBackState = UIFactory::createState(vec2(-0.05f, 0.0f), 0.0f, vec2(1.1f, 0.1f), 1.0f, "Textures/UI/FormBackground1.png");
}
//Inicializacion de los subelementos
void MenuStatusBarForm::initiateElements(){
	this->upperBar = UIFactory::createStaticImage(vec2(0.08f, 0.0f), 0.0f, vec2(1.0f, 0.1f), 1.0f, "Textures/UI/UpperBar1.png", this->uiController);	
	this->button1 = UIFactory::createButton(vec2(0.05f, 0.0f), 0.0f, vec2(0.1f, 0.1f), "<", 0.25f, vec2(0.025f, 0.008f), "./Textures/UI/Button1.png", "Textures/UI/font2.png", this->uiController);
}

void MenuStatusBarForm::updateBarDisplayData(){
	// Agregar aca todos los componentes que tenga el bar
}

void MenuStatusBarForm::bindElementsToStates(){
	// Asigno a cada estado los elementos que contendran
	this->state1->addElement(this->button1);
		
	//Inicializo el estado inicial
	this->internalState->makeStateTransition(this->state1);
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void MenuStatusBarForm::handleEvents(){
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;

	//Compruebo de que elemento se trata el evento y lo proceso	
	if(this->onFocusEvent.getAfectedElement() == this->button1 && this->onFocusEvent.getStateCode() == ONFOCUSRELEASE_UIEVENTCODE){
		this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
		this->processBackButtonEvent();
	}
	//Vacio el evento
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

MenuStatusBarForm::~MenuStatusBarForm(){
	delete this->state1;
    delete this->noBackState;
	delete this->upperBar;
	delete this->button1;
}

void MenuStatusBarForm::evaluateBackButtonDisplay(){
    /* REVIEW: Descomentar si se quiere que no se muestre el boton de back cuando no hay pantallas en el stack (queda feo igual)
    // Si la pila de pantallas esta vacia, hacemos desaparecer al boton de back
    if(this->previousForms.size() == 0){
        this->internalState->makeStateTransition(this->noBackState);
    }
    else{
        this->internalState->makeStateTransition(this->state1);
    }
     */
}

void MenuStatusBarForm::processBackButtonEvent(){
	// Cerramos el teclado virtual (por las dudas que este abierto)
	FormsManager::getInstance()->virtualKeyboardForm->setVisible(false);

	// Vuelvo a la pantalla anterior
	if(this->previousForms.size() > 0){
		if(this->actualForm != NULL){
			this->actualForm->setVisible(false);
		}
		this->actualForm = this->previousForms.back();
		this->previousForms.pop_back();
		this->actualForm->setVisible(true);		
	}
    this->evaluateBackButtonDisplay();
}

void MenuStatusBarForm::addFormStackRegister(UIForm* actualForm, UIForm* nextForm){
	this->previousForms.push_back(actualForm); 
	this->actualForm = nextForm;
    this->evaluateBackButtonDisplay();
}

void MenuStatusBarForm::clearStackRegister(){
	this->previousForms.clear();
	this->actualForm = NULL;
    this->evaluateBackButtonDisplay();
}

