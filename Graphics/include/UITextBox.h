/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UiLabel.h"

class UITextBox : public UIElement{

protected:
	//Elementos
	UiLabel* label;	
	string text, hint;
	
	//Consigue el texto que se mostrara en pantalla a partir del displaySize
	void generateDisplayText();
	//Evalua si se movio el cursor (las teclas de direccion) y actualiza el display y puntero
	bool evaluateCursorMove(int key);
	//Evalua si se apreto backspace y se actua si ocurrio
	bool evaluateBackspacePress(int key);
	//Agrega una letra en la posicion en donde esta el cursor
	void addLetterInCursorPosition(int key);
	//Setea tanto el texto real como el display text, sin modificar el cursor ni la ventana de display
	void setTextAndDisplayText(string text);

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
	void disableHint();

	//Otros atributos
	const char* filename;
	
	int displaySize; // Indica la catidad de caracteres a mostrar en el componenete
	int maxSize; // Indica la cantidad maxima de caracteres que se pueden ingresar
	unsigned int characterPointer; //Indica sobre que caracter se esta posicionand
	unsigned int displayStartPosition; // Indica de donde empieza la ventana de display
		
public:
	UITextBox(const char* filename, UIController* uiController);
	virtual ~UITextBox();

	// Modifica propiedades del texto del textbox, como el tamaño y separacion
	void setTextSize(float size){ this->label->setSize(size); }
	void setLetterSeparation(float separation){ this->label->setLetterSeparationFactor(separation); }
	
	// Indica la cantidad maxima de caracteres que se pueden ingresar
	void setMaxSize(int value){ this->maxSize = value; }
	int getMaxSize(){ return this->maxSize; }

	// Indica la catidad de caracteres a mostrar en el componenete
	void setDisplaySize(int value){ this->displaySize = value; }
	int getDisplaySize(){ return this->displaySize; }

	void onKeyDown(int key);
	void onKeyUp(int key);

	// Accessors para el texto del textbox
	string getText();
	void setText(string text);

	// Setea el label inicial del text box (hint)
	void setHintText(string text);	
};