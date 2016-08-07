/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UIVirtualKeyboard.h"
#include "UIFactory.h"
#include "FrameBufferManager.h"
#include "GraphicDevice.h"
#include "UIController.h"
#include "TextureFilterRequest.h"

// Define la cantidad de botones "normales" en la pantalla
#define MAX_BUTTONS_PER_SCREEN 26  

// Define cual es la posicion entre un boton y otro en x e y
#define BUTTONS_X_POS_INCREMENT 0.1f
#define BUTTONS_Y_POS_INCREMENT 0.1f 

char lowerCaseLetters[MAX_BUTTONS_PER_SCREEN];
char upperCaseLetters[MAX_BUTTONS_PER_SCREEN];
char symbolLetters[MAX_BUTTONS_PER_SCREEN];

UIVirtualKeyboard::UIVirtualKeyboard(string normalButtonsBackground, string specialButtonsBackground, UIController* uiController) : UIElement(uiController){
	// Contruimos las letras del diccionario
	buildDiccionary();

	this->focusedElement = NULL;
	this->isLowerCaseActive = true;
	this->isUpperCaseActive = false;
	this->lastKeyWasEnter = false;
	this->normalButtonsBackground = normalButtonsBackground;
	this->specialButtonsBackground = specialButtonsBackground;
	
	//Como no es un form, tengo que inicializar a mano
	this->initiate();
}

//Incializacion de los estados del elemento 
void UIVirtualKeyboard::initiateStates(){		
	this->showedState = UIFactory::createState(vec2(0.0f), 0.0f, vec2(1.0f), 1.0f, "./Textures/UI/transparent.png");		
}
//Inicializacion de los subelementos
void UIVirtualKeyboard::initiateElements(){
	// Iniciamos los botones normales del teclado y les definimos su posicion
	vec2 pos = vec2(0.0f);
	for(unsigned int i=0; i < MAX_BUTTONS_PER_SCREEN; i++){
		this->buttons.push_back(UIFactory::createButton(pos, 0.0f, vec2(0.1f, 0.1f), "", 0.16f, vec2(0.035f, 0.025f), this->normalButtonsBackground.c_str(), "Textures/UI/font2.png", this->uiController));
		pos.x += BUTTONS_X_POS_INCREMENT;
		if(i == 9){
			pos.x = 0.5f * BUTTONS_X_POS_INCREMENT;
			pos.y += BUTTONS_Y_POS_INCREMENT;
		}
		else if(i == 18){
			pos.x = 1.5f * BUTTONS_X_POS_INCREMENT;
			pos.y += BUTTONS_Y_POS_INCREMENT;
		}		
	}
	// Inicializamos los botones especiales
	this->shiftButton = UIFactory::createButton(vec2(0.0f, 0.2f), 0.0f, vec2(0.15f, 0.1f), "Mayus", 0.16f, vec2(0.025f, 0.023f), this->specialButtonsBackground.c_str(), "Textures/UI/font2.png", this->uiController);
	this->modeChangeButton = UIFactory::createButton(vec2(0.0f, 0.3f), 0.0f, vec2(0.2f, 0.1f), "123", 0.16f, vec2(0.06f, 0.025f), this->specialButtonsBackground.c_str(), "Textures/UI/font2.png", this->uiController);
	this->backspaceButton = UIFactory::createButton(vec2(0.85f, 0.2f), 0.0f, vec2(0.15f, 0.1f), "Del", 0.16f, vec2(0.04f, 0.025f), this->specialButtonsBackground.c_str(), "Textures/UI/font2.png", this->uiController);
	this->enterButton = UIFactory::createButton(vec2(0.8f, 0.3f), 0.0f, vec2(0.2f, 0.1f), "Done", 0.16f, vec2(0.06f, 0.025f), this->specialButtonsBackground.c_str(), "Textures/UI/font2.png", this->uiController);
	this->spaceButton = UIFactory::createButton(vec2(0.25f, 0.3f), 0.0f, vec2(0.5f, 0.1f), "Space", 0.16f, vec2(0.2f, 0.025f), this->normalButtonsBackground.c_str(), "Textures/UI/font2.png", this->uiController);

	// Iniciamos con el teclado en modo lowerCase
	this->presentLowerCaseKeyboard();
}

void UIVirtualKeyboard::bindElementsToStates(){
	// Asigno a cada estado los elementos que contendran
	for(unsigned int i=0; i < this->buttons.size(); i++){
		this->showedState->addElement(this->buttons[i]);	
	}
	this->showedState->addElement(this->shiftButton);
	this->showedState->addElement(this->backspaceButton);		
	this->showedState->addElement(this->enterButton);		
	this->showedState->addElement(this->modeChangeButton);		
	this->showedState->addElement(this->spaceButton);		
		
	//Inicializo el estado inicial
	this->internalState->makeStateTransition(this->showedState);	
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void UIVirtualKeyboard::handleEvents(){	
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;

	//Compruebo de que elemento se trata el evento y lo proceso	
	if( this->onFocusEvent.getStateCode() == ONFOCUSRELEASE_UIEVENTCODE ){

		this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
		this->lastKeyWasEnter = false;

		for(unsigned int i=0; i < this->buttons.size(); i++){			
			if(this->onFocusEvent.getAfectedElement() == this->buttons[i]){
				this->processButtonEvent(i);	
			}		
		}
		if(this->onFocusEvent.getAfectedElement() == this->shiftButton){
			this->processShiftEvent();	
		}	
		else if(this->onFocusEvent.getAfectedElement() == this->spaceButton){
			this->processSpaceEvent();	
		}
		else if(this->onFocusEvent.getAfectedElement() == this->enterButton){
			this->processEnterEvent();	
		}
		else if(this->onFocusEvent.getAfectedElement() == this->backspaceButton){
			this->processBackspaceEvent();	
		}
		else if(this->onFocusEvent.getAfectedElement() == this->modeChangeButton){
			this->processModeEvent();	
		}
	}
	//Vacio el evento
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

void UIVirtualKeyboard::processButtonEvent(int i){
	// Enviamos a Scaena la tecla presionada
	if(this->focusedElement != NULL){
		if(this->isLowerCaseActive){
			this->focusedElement->onKeyDown(lowerCaseLetters[i]);
		}
		else if(this->isUpperCaseActive){
			this->focusedElement->onKeyDown(upperCaseLetters[i]);
		}
		else {
			this->focusedElement->onKeyDown(symbolLetters[i]);
		}
	}
	// Si estabamos en UpperCase, volvemos a LowerCase
	if(this->isUpperCaseActive){
		this->presentLowerCaseKeyboard();
	}
}

void UIVirtualKeyboard::processShiftEvent(){
	if(this->isUpperCaseActive){
		this->presentLowerCaseKeyboard();
	}
	else{
		this->presentUpperCaseKeyboard();
	}
}
void UIVirtualKeyboard::processSpaceEvent(){
	this->focusedElement->onKeyDown(' ');
}
void UIVirtualKeyboard::processBackspaceEvent(){
	this->focusedElement->onKeyDown(8); //Ascii code para backspace
}
void UIVirtualKeyboard::processEnterEvent(){
	this->lastKeyWasEnter = true;
}
void UIVirtualKeyboard::processModeEvent(){
	if(this->isLowerCaseActive || this->isUpperCaseActive){
		this->presentSymbolKeyboard();
	}
	else{
		this->presentLowerCaseKeyboard();
	}
}

void UIVirtualKeyboard::presentKeyboard(char* diccionary){
	for(unsigned int i=0; i < MAX_BUTTONS_PER_SCREEN; i++){			
		this->buttons[i]->getLabel()->setText(StringUtils::toString(diccionary[i]));		
	}
}
void UIVirtualKeyboard::presentLowerCaseKeyboard(){
	this->presentKeyboard(lowerCaseLetters);
	this->isLowerCaseActive = true;
	this->isUpperCaseActive = false;
}
void UIVirtualKeyboard::presentUpperCaseKeyboard(){
	this->presentKeyboard(upperCaseLetters);
	this->isLowerCaseActive = false;
	this->isUpperCaseActive = true;
}
void UIVirtualKeyboard::presentSymbolKeyboard(){
	this->presentKeyboard(symbolLetters);
	this->isLowerCaseActive = false;
	this->isUpperCaseActive = false;
}

UIVirtualKeyboard::~UIVirtualKeyboard(){	
	delete this->showedState;		
	for(unsigned int i=0; i < this->buttons.size(); i++){
		delete this->buttons[i];	
	}
	delete this->shiftButton;
	delete this->backspaceButton;		
	delete this->enterButton;		
	delete this->modeChangeButton;		
	delete this->spaceButton;	
}

void UIVirtualKeyboard::buildDiccionary(){
	// Lower Case
	int i=0;
	lowerCaseLetters[i] = 'q'; i++;
	lowerCaseLetters[i] = 'w'; i++;
	lowerCaseLetters[i] = 'e'; i++;
	lowerCaseLetters[i] = 'r'; i++;
	lowerCaseLetters[i] = 't'; i++;
	lowerCaseLetters[i] = 'y'; i++;
	lowerCaseLetters[i] = 'u'; i++;
	lowerCaseLetters[i] = 'i'; i++;
	lowerCaseLetters[i] = 'o'; i++;
	lowerCaseLetters[i] = 'p'; i++;
	lowerCaseLetters[i] = 'a'; i++;
	lowerCaseLetters[i] = 's'; i++;
	lowerCaseLetters[i] = 'd'; i++;
	lowerCaseLetters[i] = 'f'; i++;
	lowerCaseLetters[i] = 'g'; i++;
	lowerCaseLetters[i] = 'h'; i++;
	lowerCaseLetters[i] = 'j'; i++;
	lowerCaseLetters[i] = 'k'; i++;
	lowerCaseLetters[i] = 'l'; i++;
	lowerCaseLetters[i] = 'z'; i++;
	lowerCaseLetters[i] = 'x'; i++;
	lowerCaseLetters[i] = 'c'; i++;
	lowerCaseLetters[i] = 'v'; i++;
	lowerCaseLetters[i] = 'b'; i++;
	lowerCaseLetters[i] = 'n'; i++;
	lowerCaseLetters[i] = 'm'; i++;

	// Upper Case
	i=0;
	upperCaseLetters[i] = 'Q'; i++;
	upperCaseLetters[i] = 'W'; i++;
	upperCaseLetters[i] = 'E'; i++;
	upperCaseLetters[i] = 'R'; i++;
	upperCaseLetters[i] = 'T'; i++;
	upperCaseLetters[i] = 'Y'; i++;
	upperCaseLetters[i] = 'U'; i++;
	upperCaseLetters[i] = 'I'; i++;
	upperCaseLetters[i] = 'O'; i++;
	upperCaseLetters[i] = 'P'; i++;
	upperCaseLetters[i] = 'A'; i++;
	upperCaseLetters[i] = 'S'; i++;
	upperCaseLetters[i] = 'D'; i++;
	upperCaseLetters[i] = 'F'; i++;
	upperCaseLetters[i] = 'G'; i++;
	upperCaseLetters[i] = 'H'; i++;
	upperCaseLetters[i] = 'J'; i++;
	upperCaseLetters[i] = 'K'; i++;
	upperCaseLetters[i] = 'L'; i++;
	upperCaseLetters[i] = 'Z'; i++;
	upperCaseLetters[i] = 'X'; i++;
	upperCaseLetters[i] = 'C'; i++;
	upperCaseLetters[i] = 'V'; i++;
	upperCaseLetters[i] = 'B'; i++;
	upperCaseLetters[i] = 'N'; i++;
	upperCaseLetters[i] = 'M'; i++;

	// Numeros y simbolos
	i=0;
	symbolLetters[i] = '1'; i++;
	symbolLetters[i] = '2'; i++;
	symbolLetters[i] = '3'; i++;
	symbolLetters[i] = '4'; i++;
	symbolLetters[i] = '5'; i++;
	symbolLetters[i] = '6'; i++;
	symbolLetters[i] = '7'; i++;
	symbolLetters[i] = '8'; i++;
	symbolLetters[i] = '9'; i++;
	symbolLetters[i] = '0'; i++;

	symbolLetters[i] = '@'; i++;
	symbolLetters[i] = '$'; i++;
	symbolLetters[i] = '!'; i++;
	symbolLetters[i] = '#'; i++;
	symbolLetters[i] = ':'; i++;
	symbolLetters[i] = '&'; i++;
	symbolLetters[i] = '_'; i++;
	symbolLetters[i] = '('; i++;
	symbolLetters[i] = ')'; i++;

	symbolLetters[i] = ';'; i++;
	symbolLetters[i] = '-'; i++;
	symbolLetters[i] = '\''; i++;
	symbolLetters[i] = '"'; i++;
	symbolLetters[i] = '.'; i++;
	symbolLetters[i] = ','; i++;
	symbolLetters[i] = '?'; i++;	
}
