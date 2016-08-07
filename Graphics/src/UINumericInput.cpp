/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UINumericInput.h"
#include <sstream>

#define BACKSPACE_KEY 8
#define DIRECTION_UP_KEY -101
#define DIRECTION_DOWN_KEY -103

#define VALUE_INCREMENT 0.1f

string modifyFloatValueUsingString(string in, float increment){
	float value = atof(in.c_str());
	value += increment;
	std::stringstream buff;
	buff << value;
	return buff.str();
}

void UINumericInput::setValue(float value){
	std::stringstream buff;
	buff << value;
	this->setText(buff.str());
}
void UINumericInput::setValue(int value){
	std::stringstream buff;
	buff << value;
	this->setText(buff.str());
}

void UINumericInput::onKeyDown(int key){
	//Verifico si se apreto backspace
	if(this->evaluateBackspacePress(key)){
		return;
	}

	//Verifico si se movio el cursor
	if(this->evaluateCursorMove(key)){
		return;
	}

	//Si se presionan las teclas de direccion se modifica el valor del input
	if(key == DIRECTION_UP_KEY){
		string stringValue = modifyFloatValueUsingString(this->getText(), VALUE_INCREMENT);	
		stringValue = this->applyPadding(stringValue);
		this->setText(stringValue);
		return;
	}
	if(key == DIRECTION_DOWN_KEY){
		string stringValue = modifyFloatValueUsingString(this->getText(), -VALUE_INCREMENT);
		stringValue = this->applyPadding(stringValue);
		this->setText(stringValue);
		return;
	}

	//Verifico no superar el largo maximo
	string actualText = this->getText();
	if(actualText.length() >= this->getMaxSize()) return;

	//Verifico que sea un numero o un punto
	if(((key >= 48)&&(key <= 57)) || (key == 46)){
		//Append del numero
		this->addLetterInCursorPosition(key);
	}	
}

string UINumericInput::applyPadding(string stringToPad){
	if(stringToPad.length() < this->getDisplaySize()){
		// Voy a buscar si el numero ya tiene punto
		unsigned int pointPosition = stringToPad.find(".");

		// Si se encontro un punto y hay algun caracter despues, termine
		if((pointPosition == string::npos) && (pointPosition + 1 < stringToPad.length() - 1)){
			return stringToPad;
		}

		// Si no encontro un punto, agrego el punto y un cero
		if(pointPosition == string::npos){
			stringToPad += ".0";
		}
	}
	return stringToPad;
}