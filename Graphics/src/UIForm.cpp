/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UIForm.h"

UIForm::UIForm(UIController* uiController) : UIElement(uiController){ 
	this->goingToFrontOnFocus = true;
	this->visible = true;		
}

// Manda a manejar todos los eventos que se hayan producido en este form o sus elementos
void UIForm::handleFormEvents(){
	this->handleElementEvents();
}