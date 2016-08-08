/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIElement.h"
#include "FrameBuffer.h"
#include <string>
using namespace std;

class UiLabel : public UIElement{

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
	
	//Estados
	UIPartialTextureState* partialLabelState;
	UIState* finalLabelState;

	//Otros atributos
	const char* filename;
	string text;
	int charactersPerLine;
	float letterSeparationFactor;
	bool textHasChanged;
	float letterHeightFactor;

	// Necesito un frame buffer para dibujar el label en memoria
	FrameBuffer* frameBuffer;
	
	// Genera el label en memoria (para dibujar en menos batch)
	void generateLabel();
	
	// Consigue un framebuffer que se adapte a lo necesitado y lo bindea
	void manageAndBindFrameBuffer(vec2 letterSizeInPixels);

	// Calcula el tamaño del label final (con todoas las letras) en pixels y relativo a la pantalla
	vec2 calculateLabelSizeInPixels(vec2 pixelsPerLetter);
	
	// Consigue el tamaño relativo a la pantalla de la letra
	vec2 getRelativeLetterSize();

	// Consigue el tamaño en pixels de la letra
	vec2 getLetterSizeInPixels();

	// Dada una escala consigue la componente menor y con ella hace una escala cuadrada
	vec2 getLowerScaleComponent(vec2 scale);
		
public:
	UiLabel(const char* filename, UIController* uiController);
	virtual ~UiLabel();

	void draw();
	void update(UIState* externalState);

	//Accesors
	void setText(string text);
	string getText(){ return this->text; }

	void setCharactersPerLine(int value){ this->charactersPerLine = value; }
	int getCharactersPerLine(){ return this->charactersPerLine; }

	void setLetterSeparationFactor(float value){ this->letterSeparationFactor = value; }
	float getLetterSeparationFactor(){ return this->letterSeparationFactor; }

	void setSize(float size){ this->externalState->getActualState()->setScale(vec2(size)); }

	void setLetterHeightFactor(float factor){ this->letterHeightFactor = factor; }

	// Si se necesita generar texto pero para una textura en vez de para UI, usar este metodo
	Texture* getLabelTexture();
};