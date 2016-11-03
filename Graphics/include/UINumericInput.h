/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UITextBox.h"

class UINumericInput : public UITextBox{

private:
	string applyPadding(string stringToPad);
		
public:
	UINumericInput(const char* filename, UIController* uiController) : UITextBox(filename, uiController){};
	virtual ~UINumericInput(){}

	//Override
	void onKeyDown(int key);

	float getValueAsFloat(){ return (float) atof(this->getText().c_str()); }
	int getValueAsInteger(){ return atoi(this->getText().c_str()); }
	void setValue(float value);
	void setValue(int value);
};