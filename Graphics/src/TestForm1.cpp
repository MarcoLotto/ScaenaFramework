/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "TestForm1.h"

TestForm1::TestForm1(UIController* uiController) : UIForm(uiController){
	this->initiate();	
}

//Incializacion de los estados del elemento 
void TestForm1::initiateStates(){
	this->basicState = UIFactory::createState(vec2(0.2f, 0.2f), 0.0f, vec2(0.7f, 0.7f), 1.0f, 
		"./Textures/UI/test1.png");
	this->transformedState = UIFactory::createState(vec2(0.2f, 0.2f), 0.0f, vec2(0.6f, 0.6f), 1.0f, 
		"./Textures/UI/test1.png");
	this->dynamicState = UIFactory::createDynamicTextureState(vec2(0.2f, 0.2f), 0.0f, vec2(0.5f, 0.5f), 1.0f,
		"./Textures/UI/dwarf.png", vec2(0.1f, 0.03125f), vec2(0.0f, 0.0f), 12, 6000, true);
	this->holdingState = UIFactory::createState(vec2(0.2f, 0.2f), 0.0f, vec2(0.5f, 0.5f), 1.0f, 
		"./Textures/UI/happyFace.png");
}
//Inicializacion de los subelementos
void TestForm1::initiateElements(){
	this->button1 = UIFactory::createButton(vec2(0.1f, 0.0f), 90.0f, vec2(0.4f, 0.1f), "Buenos dias", 0.1f, "./Textures/UI/fullButton.png", this->uiController);
	this->textBox1 = UIFactory::createTextBox(vec2(0.0f, 0.3f), 0.0f, vec2(0.4f, 0.3f), "...", 0.1f, 30, 19, "./Textures/UI/textbox1.png", this->uiController);	
	this->label1 = UIFactory::createLabel("Label 1", vec2(0.0f), 0.1f, "./Textures/UI/font2.png", this->uiController);
	this->label2 = UIFactory::createLabel("Label 2", vec2(0.0f), 0.1f, "./Textures/UI/font2.png", this->uiController);
}

void TestForm1::bindElementsToStates(){
	this->basicState->addElement(this->button1);
	this->transformedState->addElement(this->button1);
	this->dynamicState->addElement(this->button1);
	this->holdingState->addElement(this->button1);

	this->basicState->addElement(this->textBox1);
	this->transformedState->addElement(this->textBox1);
	this->dynamicState->addElement(this->textBox1);
	this->holdingState->addElement(this->textBox1);
	
	//Inicializo los estados iniciales
	this->internalState->makeStateTransition(this->basicState);
	this->internalState->makeStateTransition(this->transformedState, 4000, true);
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void TestForm1::handleEvents(){
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;
	
	//Compruebo de que elemento se trata el evento y lo proceso
	if(this->onFocusEvent.getAfectedElement() == this){
		this->processEvent1();	
	}	
	if(this->onFocusEvent.getAfectedElement() == this->button1){
		this->processEvent2();	
	}
}


void TestForm1::processEvent2(){
	if(this->onFocusEvent.getStateCode() == ONFOCUSRELEASE_UIEVENTCODE){
		static bool basicTransitionEnabled = false;
		if(basicTransitionEnabled){
			this->internalState->makeStateTransition(this->dynamicState, this->basicState, 4000, true);
			basicTransitionEnabled = false;
		}
		else{
			this->internalState->makeStateTransition(this->basicState, this->transformedState, 4000, true);
			basicTransitionEnabled=true;
		}
	}
	//Vacio el evento (No me interesa en este caso mantener estados de hold ni nada de eso)
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

void TestForm1::processEvent1(){
	if(this->onFocusEvent.getStateCode() == ONFOCUSHOLD_UIEVENTCODE){
		this->internalState->makeStateTransition(this->holdingState);
		return;
	}
	//Vacio el evento (No me interesa en este caso mantener estados de hold ni nada de eso)
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

TestForm1::~TestForm1(){
	//Borro los estados
	if(this->basicState != NULL){
		delete this->basicState;
		this->basicState = NULL;
	}
	if(this->transformedState != NULL){
		delete this->transformedState;
		this->transformedState = NULL;
	}
	if(this->dynamicState != NULL){
		delete this->dynamicState;
		this->dynamicState = NULL;
	}
	if(this->holdingState != NULL){
		delete this->holdingState;
		this->holdingState = NULL;
	}
	//Borro los componentes
	if(this->button1 != NULL){
		delete this->button1;
		this->button1 = NULL;
	}
}