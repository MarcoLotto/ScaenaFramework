/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIElement.h"
#include "UiLabel.h"
#include "UIStaticImage.h"

class UiButton : public UIElement{

protected:
	//Maneja los eventos que se producen cuando un elemento consigue el foco
	void handleEvents();

private:
	//Incializacion de los estados del elemento 
	void initiateStates();
	//Inicializacion de los subelementos
	void initiateElements();
	//Linkea los elementos con los estados (cuales deben ser dibujados en cada estado)
	void bindElementsToStates();

	void initButton(const char* backgroundFilename, const char* font, vec2 textPos, const char* imageFilename, vec2 imgPos, vec2 imgScale, UIController* uiController);

	//Estados
	UIState* basicState;
	UIState* pressState;
	UIState* mouseOnState;

	//Elementos
	UiLabel* label;
	UIStaticImage* image;

	//Otros atributos y propiedades
	const char* filename;
	const char* font;
	vec2 textPos;
	vec2 imgPos;
	vec2 imgScale;
	const char* imageFilename;
	bool usesImage();
	bool usesLabel();
	
	//Auxiliares para procesamientos de eventos
	void processPressEvent();
	void processMouseOnEvent();
	void processReleaseEvent();

public:
	UiButton(const char* backgroundFilename, UIController* uiController);
	UiButton(const char* backgroundFilename, const char* font, vec2 textPos, UIController* uiController);
	UiButton(const char* backgroundFilename, const char* font, vec2 textPos, const char* imageFilename, vec2 imgPos, vec2 imgScale, UIController* uiController);
	UiButton(const char* backgroundFilename, const char* imageFilename, vec2 imgPos, vec2 imgScale, UIController* uiController);
	virtual ~UiButton();

	UiLabel* getLabel(){ return this->label; }
	UIStaticImage* getImage(){ return this->image; }


};