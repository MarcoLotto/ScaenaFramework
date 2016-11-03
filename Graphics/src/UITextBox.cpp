/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UITextBox.h"
#include "UIFactory.h"

#define BACKSPACE_KEY 8
#define LEFT_ARROW_KEY -100
#define RIGHT_ARROW_KEY -102

UITextBox::UITextBox(const char* filename, UIController* uiController) : UIElement(uiController){
	//La ruta de la imagen a cargar
	this->filename = filename;
	this->maxSize = 0;
	this->displaySize = 0;
	this->characterPointer = 0;
	this->displayStartPosition = 0;

	//Como no es un form, tengo que inicializar a mano
	this->initiate();
}

//Incializacion de los estados del elemento 
void UITextBox::initiateStates(){
	this->basicState = UIFactory::createState(vec2(0.0f), 0.0f, vec2(1.0f, 0.3f), 1.0f, this->filename);	
}

//Inicializacion de los subelementos
void UITextBox::initiateElements(){
	//Los valores de tamaño y separacion entre letras no son importantes aqui
	this->label = UIFactory::createLabel("", vec2(0.01f, 0.01f), 0.1f, "./Textures/UI/font2.png", this->uiController);
}

void UITextBox::bindElementsToStates(){
	this->basicState->addElement(this->label);
	
	this->internalState->makeStateTransition(this->basicState);	
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void UITextBox::handleEvents(){
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;

	//Si solte el click, borro el hint
	if( this->onFocusEvent.getStateCode() == ONFOCUSRELEASE_UIEVENTCODE ){
		this->disableHint();
	}
	//Vacio el evento
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

UITextBox::~UITextBox(){
	if(this->basicState != NULL){
		delete this->basicState;
		this->basicState = NULL;
	}
	if(this->label != NULL){
		delete this->label;
		this->label = NULL;
	}
}

void UITextBox::onKeyDown(int key){
	//Verifico si se apreto backspace
	if(this->evaluateBackspacePress(key)){
		return;
	}

	//Verifico si se movio el cursor
	if(this->evaluateCursorMove(key)){
		return;
	}

	//Verifico no superar el largo maximo
	string actualText = this->text;
	if(actualText.length() >= this->maxSize) return;

	//Append de la letra
	this->addLetterInCursorPosition(key);
}

void UITextBox::addLetterInCursorPosition(int key){
	// Agrego la letra en la posicion del cursor
	string letter = string(1, (unsigned char)key);
	string firstPart = this->text.substr(0, this->characterPointer);
	string secondPart = this->text.substr(this->characterPointer, this->text.length());
	this->setTextAndDisplayText(firstPart + letter + secondPart);

	//Como se agrego un numero, avanzo el cursor
	this->characterPointer++;
}

void UITextBox::onKeyUp(int key){	
}

int valueModule(int value){
	if(value < 0)
		return 0;
	return value;	
}

string UITextBox::getText(){ 
	if(this->hint.empty())
		return this->text;
	return "";
}

void UITextBox::setText(string text){
	// Primero actualizo las variables de la ventana de display
	this->characterPointer = text.length(); // El cursor al final del texto
	this->displayStartPosition = valueModule(text.length() - this->displaySize); // Empiezo a ver desde el final
	this->setTextAndDisplayText(text);
}

void UITextBox::setHintText(string text){ 
	this->setText(text);
	this->hint = text; 
}

void UITextBox::disableHint(){
	if(!this->hint.empty()){
		this->hint = "";
		this->setText(this->hint);
	}
}

void UITextBox::setTextAndDisplayText(string text){
	this->text = text;
	this->generateDisplayText();
}

void UITextBox::generateDisplayText(){
	//Verifico si tengo que mover la ventana de diplay
	unsigned int displayEndPosition = this->displayStartPosition + this->displaySize;
	if(this->characterPointer > displayEndPosition){
		this->displayStartPosition++;
	}
	else if(this->characterPointer < displayStartPosition){
		this->displayStartPosition--;
	}
	
	//Consigo y guardo el texto que se muestra dependiendo de la ventana de texto
	string displayText = this->text.substr(this->displayStartPosition, this->displaySize);
	this->label->setText(displayText);
}

bool UITextBox::evaluateCursorMove(int key){
	//Evaluo si se movio el cursor y actualizo la posicion del puntero
	bool cursorHasMove = false;
	if(key == RIGHT_ARROW_KEY){
		if(this->characterPointer < this->text.length()){
			this->characterPointer++;
		}
		cursorHasMove = true;
	}
	if(key == LEFT_ARROW_KEY){
		if(this->characterPointer > 0){
			this->characterPointer--;
		}
		cursorHasMove = true;
	}
	// Si el cursor se movio, actualizo el texto que se muestra al usuario 
	if(cursorHasMove){
		this->generateDisplayText();
	}
	return cursorHasMove;
}

bool UITextBox::evaluateBackspacePress(int key){
	//Si se apreto backspace borro una letra
	if(key == BACKSPACE_KEY){
		string actualText = this->getText();
		unsigned int initialPosition = 0;
		if(this->characterPointer > 0){
			initialPosition = this->characterPointer-1;
		}
		// Si mi label supera el tamaño de la ventana, lo voy corriendo
		if(this->displayStartPosition > 0){
			this->displayStartPosition--;
		}
		string firstPart = actualText.substr(0, initialPosition);
		string secondPart = actualText.substr(this->characterPointer, actualText.length());
		this->setTextAndDisplayText(firstPart + secondPart);
		
		//Actualizo la posicion del puntero
		if(this->characterPointer > 0){
			this->characterPointer--;
		}
		return true;
	}
	return false;
}
