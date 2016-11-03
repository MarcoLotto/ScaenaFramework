/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIDynamicTextureState.h"
#include "Scene.h"

class UIController;
class UiButton;
class UiLabel;
class UITextBox;
class UIScrollBar;
class UIList;
class UINumericInput;
class UICheckBox;
class UIStaticImage;
class Ui3dDrawable;
class UIDynamicImage;
class UIGraphicList;
class UIVirtualKeyboard;

class UIFactory{

private:

	static void internalCreateButton(UiButton* button, vec2 pos, float rot, vec2 scale, string label, float fontSize);


public:
	//************************************************************************************************
	//*											Estados												 *
	//************************************************************************************************
	//Crea un estado comun, de imagen estatica
	static UIState* createState(vec2 pos, float rot, vec2 scale, float transp);
	static UIState* createState(vec2 pos, float rot, vec2 scale, float transp, const char* filename);

	//Crea un estado seleccionando el sector de imagen a mostrar
	static UIPartialTextureState* createPartialTextureState(vec2 pos, float rot, vec2 scale, float transp, const char* filename, 
		vec2 partialSize, vec2 posIndex);

	static UIDynamicTextureState* createDynamicTextureState(vec2 pos, float rot, vec2 scale, float transp, const char* filename, 
		vec2 partialSize, vec2 posIndex, int rowsToIterate, int animationTime, bool animationLoop);

	//Devuelve un estado basico. Este estado solo se crea una vez, siempre es la misma instancia.
	static UIState* getBasicState();

	//************************************************************************************************
	//*											Componetes											 *
	//************************************************************************************************
	static UIStaticImage* createStaticImage(vec2 pos, float rot, vec2 scale, float transparency, const char* filename, UIController* uiController);
	static UIDynamicImage* createDynamicImage(vec2 pos, float rot, vec2 scale, float transparency, const char* filename, int animationRows, int timeMillisecons, bool loop, UIController* uiController);
	static UiButton* createButton(vec2 pos, float rot, vec2 scale, string label, float fontSize, const char* filename, UIController* uiController);
	static UiButton* createButton(vec2 pos, float rot, vec2 scale, string label, float fontSize, vec2 fontPos, const char* filename, const char* font, UIController* uiController);
	static UiButton* createImageAndLabelButton(vec2 pos, float rot, vec2 scale, string label, float fontSize, vec2 fontPos, const char* filename, const char* font, const char* image, vec2 imgPos, vec2 imgSize, UIController* uiController);
	static UiButton* createImageButton(vec2 pos, float rot, vec2 scale, const char* filename, const char* image, vec2 imgPos, vec2 imgSize, UIController* uiController);
	static UiLabel* createLabel(string text, vec2 pos, float fontSize, const char* fontFilename, UIController* uiController);
	static UITextBox* createTextBox(vec2 pos, float rot, vec2 scale, string defaultText, float fontSize, int maxSize, int displaySize, const char* filename, UIController* uiController);
	static UINumericInput* createNumericInput(vec2 pos, float rot, vec2 scale, string defaultText, float fontSize, int maxSize, int displaySize, const char* filename, UIController* uiController);
	static UIScrollBar* createScrollBar(vec2 pos, float rot, float width, float barLength, const char* filename, UIController* uiController);
	static UIList* createList(vec2 pos, float rot, vec2 scale, float displayElementsCount, const char* filename, UIController* uiController);
	static UICheckBox* createCheckBox(vec2 pos, float rot, vec2 scale, const char* filename, UIController* uiController);
	static Ui3dDrawable* create3dDrawable(vec2 pos, float rot, vec2 scale, Scene* scene, UIController* uiController);
	static UIGraphicList* createGraphicList(vec2 pos, float rot, vec2 scale, int showedOptionsCount, string upButtonBackground, string downButtonBackground, string listBackground, vec2 labelPos, float labelSize, vec2 imgPos, vec2 imgSize, UIController* uiController);
	static UIGraphicList* createGraphicList(vec2 pos, float rot, vec2 scale, int showedOptionsCount, string upButtonBackground, string downButtonBackground, string listBackground, vec2 imgPos, vec2 imgSize, UIController* uiController);
	static UIVirtualKeyboard* createVirtualKeyboard(vec2 pos, float rot, vec2 scale, string normalButtonsBackground, string specialButtonsBackground, UIController* uiController);
};