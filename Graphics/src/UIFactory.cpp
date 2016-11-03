/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include <string.h>
#include "UIFactory.h"
#include "UiButton.h"
#include "UiLabel.h"
#include "UITextBox.h"
#include "UIScrollBar.h"
#include "UIList.h"
#include "UINumericInput.h"
#include "UICheckBox.h"
#include "FileTexture.h"
#include "UIController.h"
#include "UIStaticImage.h"
#include "Ui3dDrawable.h"
#include "UIDynamicImage.h"
#include "UIGraphicList.h"
#include "UIVirtualKeyboard.h"

#define DEFAULT_LETTER_SEPARATION_FACTOR 2.0f

UIState* UIFactory::createState(vec2 pos, float rot, vec2 scale, float transp){
	return createState(pos, rot, scale, transp, NULL);
}
UIState* UIFactory::createState(vec2 pos, float rot, vec2 scale, float transp, const char* filename){
	UIState* state = new UIState();
	state->setPosition(pos);
	state->setRotation(rot);
	state->setScale(scale);
	state->setTransparency(transp);

	//Se esta cargando una textura, asegurarse que funcione la "cache" de texturas
	FileTexture* texture = NULL;
	if(filename != NULL && strlen(filename) > 0){
		texture = new FileTexture();
		texture->loadFromFile(filename);
	}
	state->setTexture(texture);

	return state;
}

UIPartialTextureState* UIFactory::createPartialTextureState(vec2 pos, float rot, vec2 scale, float transp, const char* filename, 
		vec2 partialSize, vec2 posIndex){
	UIPartialTextureState* state = new UIPartialTextureState();
	state->setPosition(pos);
	state->setRotation(rot);
	state->setScale(scale);
	state->setTransparency(transp);
	state->setPartialSize(partialSize);
	state->setPositionIndex(posIndex);

	//Se esta cargando una textura, asegurarse que funcione la "cache" de texturas
	FileTexture* texture = NULL;
	if(filename != NULL && strlen(filename) > 0){
		texture = new FileTexture();
		texture->loadFromFile(filename);
	}
	state->setTexture(texture);

	return state;
}

UIDynamicTextureState* UIFactory::createDynamicTextureState(vec2 pos, float rot, vec2 scale, float transp, const char* filename, 
		vec2 partialSize, vec2 posIndex, int rowsToIterate, int animationTime, bool animationLoop){
	UIDynamicTextureState* state = new UIDynamicTextureState();
	state->setPosition(pos);
	state->setRotation(rot);
	state->setScale(scale);
	state->setTransparency(transp);
	state->setPartialSize(partialSize);
	state->setPositionIndex(posIndex);
	state->setAnimationLoop(animationLoop);
	state->setAnimationTime(animationTime);
	state->setRowsToIterate(rowsToIterate);

	//Se esta cargando una textura, asegurarse que funcione la "cache" de texturas
	FileTexture* texture = NULL;
	if(filename != NULL && strlen(filename) > 0){
		texture = new FileTexture();
		texture->loadFromFile(filename);
	}
	state->setTexture(texture);

	return state;
}

UIState* UIFactory::getBasicState(){
	//Creo un estado basico y marco que tiene que ser destruido luego de una transicion
	UIState* basicState = createState(vec2(0.0f), 0.0f, vec2(1.0f), 1.0f, NULL);
	basicState->setDestroyedAfterTransition(true);	
	return basicState;
}

UIStaticImage* UIFactory::createStaticImage(vec2 pos, float rot, vec2 scale, float transparency, const char* filename, UIController* uiController){
	UIStaticImage* image = new UIStaticImage(filename, uiController);	
	//Creo un estado externo
	UIState* externalState = UIFactory::createState(pos, rot, scale, transparency, NULL);
	//Informo que debe destruirse el estado luego de efectuar una transicion
	externalState->setDestroyedAfterTransition(true);
	//Asigno el estado y termine
	image->getExternalState()->makeStateTransition(externalState);
	return image;
}

UIDynamicImage* UIFactory::createDynamicImage(vec2 pos, float rot, vec2 scale, float transparency, const char* filename, int animationRows, int timeMillisecons, bool loop, UIController* uiController){
	UIDynamicImage* image = new UIDynamicImage(filename, uiController, animationRows, timeMillisecons, loop);	
	//Creo un estado externo
	UIState* externalState = UIFactory::createState(pos, rot, scale, transparency, NULL);
	//Informo que debe destruirse el estado luego de efectuar una transicion
	externalState->setDestroyedAfterTransition(true);
	//Asigno el estado y termine
	image->getExternalState()->makeStateTransition(externalState);
	return image;
}

void UIFactory::internalCreateButton(UiButton* button, vec2 pos, float rot, vec2 scale, string label, float fontSize){
	UiLabel* buttonLabel = button->getLabel();
	if(buttonLabel != NULL){
		buttonLabel->setText(label);
		buttonLabel->setSize(fontSize);
		buttonLabel->setLetterSeparationFactor(DEFAULT_LETTER_SEPARATION_FACTOR);
	}
	//Creo un estado externo para el boton
	UIState* externalState = UIFactory::createState(pos, rot, scale, 1.0f, NULL);
	//Informo que debe destruirse el estado luego de efectuar una transicion
	externalState->setDestroyedAfterTransition(true);
	//Asigno el estado
	button->getExternalState()->makeStateTransition(externalState);
}

UiButton* UIFactory::createButton(vec2 pos, float rot, vec2 scale, string label, float fontSize, const char* filename, UIController* uiController){
	UiButton* button = new UiButton(filename, uiController);
	internalCreateButton(button, pos, rot, scale, label, fontSize);
	return button;
}

// Boton con label pero sin imagen
UiButton* UIFactory::createButton(vec2 pos, float rot, vec2 scale, string label, float fontSize, vec2 fontPos, const char* filename, const char* font, UIController* uiController){
	UiButton* button = new UiButton(filename, font, fontPos, uiController);
	internalCreateButton(button, pos, rot, scale, label, fontSize);
	return button;
}

// Boton con imagen y label
UiButton* UIFactory::createImageAndLabelButton(vec2 pos, float rot, vec2 scale, string label, float fontSize, vec2 fontPos, const char* filename, const char* font, const char* image, vec2 imgPos, vec2 imgSize, UIController* uiController){
	UiButton* button = new UiButton(filename, font, fontPos, image, imgPos, imgSize, uiController);
	internalCreateButton(button, pos, rot, scale, label, fontSize);
	return button;
}

// Boton con imagen pero sin label
UiButton* UIFactory::createImageButton(vec2 pos, float rot, vec2 scale, const char* filename, const char* image, vec2 imgPos, vec2 imgSize, UIController* uiController){
	UiButton* button = new UiButton(filename, image, imgPos, imgSize, uiController);
	internalCreateButton(button, pos, rot, scale, "", 0.0f);
	return button;
}

UICheckBox* UIFactory::createCheckBox(vec2 pos, float rot, vec2 scale, const char* filename, UIController* uiController){
	UICheckBox* checkbox = new UICheckBox(filename, uiController);	
	//Creo un estado externo
	UIState* externalState = UIFactory::createState(pos, rot, scale, 1.0f, NULL);
	//Informo que debe destruirse el estado luego de efectuar una transicion
	externalState->setDestroyedAfterTransition(true);
	//Asigno el estado
	checkbox->getExternalState()->makeStateTransition(externalState);
	return checkbox;
}

UiLabel* UIFactory::createLabel(string text, vec2 pos, float fontSize, const char* fontFilename, UIController* uiController){
	UiLabel* label = new UiLabel(fontFilename, uiController);
	label->setText(text);
	//Creo el estado y lo asigno
	UIState* externalState = UIFactory::createState(pos, 0.0f, vec2(fontSize), 1.0f, NULL);
	externalState->setDestroyedAfterTransition(true);
	label->getExternalState()->makeStateTransition(externalState);
	return label;
}

UITextBox* UIFactory::createTextBox(vec2 pos, float rot, vec2 scale, string defaultText, float fontSize, int maxSize, int displaySize, const char* filename, UIController* uiController){
	UITextBox* textbox = new UITextBox(filename, uiController);
	textbox->setMaxSize(maxSize);
	textbox->setDisplaySize(displaySize);
	textbox->setHintText(defaultText);
	textbox->setTextSize(fontSize);
	textbox->setLetterSeparation(DEFAULT_LETTER_SEPARATION_FACTOR);
	//Creo un estado externo para el textbox
	UIState* externalState = UIFactory::createState(pos, rot, scale, 1.0f, NULL);
	//Informo que debe destruirse el estado luego de efectuar una transicion
	externalState->setDestroyedAfterTransition(true);
	//Asigno el estado
	textbox->getExternalState()->makeStateTransition(externalState);
	//Habemus boton
	return textbox;
}

UINumericInput* UIFactory::createNumericInput(vec2 pos, float rot, vec2 scale, string defaultText, float fontSize, int maxSize, int displaySize, const char* filename, UIController* uiController){
	UINumericInput* textbox = new UINumericInput(filename, uiController);
	textbox->setMaxSize(maxSize);
	textbox->setDisplaySize(displaySize);
	textbox->setText(defaultText);
	textbox->setTextSize(fontSize);
	textbox->setLetterSeparation(DEFAULT_LETTER_SEPARATION_FACTOR);
	//Creo un estado externo para el textbox
	UIState* externalState = UIFactory::createState(pos, rot, scale, 1.0f, NULL);
	//Informo que debe destruirse el estado luego de efectuar una transicion
	externalState->setDestroyedAfterTransition(true);
	//Asigno el estado
	textbox->getExternalState()->makeStateTransition(externalState);
	//Habemus boton
	return textbox;
}

UIScrollBar* UIFactory::createScrollBar(vec2 pos, float rot, float width, float barLength, const char* filename, UIController* uiController){
	UIScrollBar* scrollbar = new UIScrollBar(filename, uiController);
	//Creo un estado externo para el boton
	UIState* externalState = UIFactory::createState(pos, rot, vec2(1.0f), 1.0f, NULL);
	//Informo que debe destruirse el estado luego de efectuar una transicion
	externalState->setDestroyedAfterTransition(true);
	//Asigno el estado
	scrollbar->getExternalState()->makeStateTransition(externalState);
	//Pongo los largos y anchos correctos
	scrollbar->setWidth(width);
	scrollbar->setHeight(barLength);
	//Habemus boton
	return scrollbar;
}

UIList* UIFactory::createList(vec2 pos, float rot, vec2 scale, float displayElementsCount, const char* filename, UIController* uiController){
	UIList* list = new UIList(filename, displayElementsCount, scale.y, uiController);
	//Le seteo la posicion al scrollBar en X para que se ajuste al ancho de la lista
	list->setScrollBarPositionX(scale.x);
	//Creo un estado externo
	UIState* externalState = UIFactory::createState(pos, rot, scale, 1.0f, NULL);
	//Informo que debe destruirse el estado luego de efectuar una transicion
	externalState->setDestroyedAfterTransition(true);
	//Asigno el estado
	list->getExternalState()->makeStateTransition(externalState);
	//Habemus boton
	return list;
}

Ui3dDrawable* UIFactory::create3dDrawable(vec2 pos, float rot, vec2 scale, Scene* scene, UIController* uiController){
	//Creo un estado externo para el componente y se lo asigno
	UIState* externalState = UIFactory::createState(pos, rot, scale, 1.0f, NULL);	
	externalState->setDestroyedAfterTransition(true);	
	Ui3dDrawable* drawer = new Ui3dDrawable(scene, uiController);	
	drawer->getExternalState()->makeStateTransition(externalState);
	return drawer;
}

UIGraphicList* UIFactory::createGraphicList(vec2 pos, float rot, vec2 scale, int showedOptionsCount, string upButtonBackground, string downButtonBackground, 
								 string listBackground, vec2 labelPos, float labelSize, vec2 imgPos, vec2 imgSize, UIController* uiController){
	UIGraphicList* graphicList = new UIGraphicList(uiController, scale, showedOptionsCount, upButtonBackground, downButtonBackground, listBackground, labelPos, labelSize, imgPos, imgSize);	
	//Creo un estado externo
	UIState* externalState = UIFactory::createState(pos, rot, vec2(1.0f), 1.0f, NULL);
	//Informo que debe destruirse el estado luego de efectuar una transicion
	externalState->setDestroyedAfterTransition(true);
	//Asigno el estado
	graphicList->getExternalState()->makeStateTransition(externalState);
	return graphicList;
}

UIGraphicList* UIFactory::createGraphicList(vec2 pos, float rot, vec2 scale, int showedOptionsCount, string upButtonBackground, string downButtonBackground, 
								 string listBackground, vec2 imgPos, vec2 imgSize, UIController* uiController){
	UIGraphicList* graphicList = new UIGraphicList(uiController, scale, showedOptionsCount, upButtonBackground, downButtonBackground, listBackground, imgPos, imgSize);	
	//Creo un estado externo
	UIState* externalState = UIFactory::createState(pos, rot, vec2(1.0f), 1.0f, NULL); 
	//Informo que debe destruirse el estado luego de efectuar una transicion
	externalState->setDestroyedAfterTransition(true);
	//Asigno el estado
	graphicList->getExternalState()->makeStateTransition(externalState);
	return graphicList;
}

UIVirtualKeyboard* UIFactory::createVirtualKeyboard(vec2 pos, float rot, vec2 scale, string normalButtonsBackground, string specialButtonsBackground, UIController* uiController){
	UIVirtualKeyboard* virtualKeyboard = new UIVirtualKeyboard(normalButtonsBackground, specialButtonsBackground, uiController);	
	//Creo un estado externo
	UIState* externalState = UIFactory::createState(pos, rot, vec2(1.0f), 1.0f, NULL); 
	//Informo que debe destruirse el estado luego de efectuar una transicion
	externalState->setDestroyedAfterTransition(true);
	//Asigno el estado
	virtualKeyboard->getExternalState()->makeStateTransition(externalState);
	return virtualKeyboard;
}
