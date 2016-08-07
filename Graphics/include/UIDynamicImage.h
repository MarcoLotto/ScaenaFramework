/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIElement.h"
#include "UIDynamicTextureState.h"

class UIDynamicImage : public UIElement{

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
	UIDynamicTextureState* state;

	int animationRows;
	int animationTime;
	bool loop;

	//Otros atributos
	const char* filename;

public:
	UIDynamicImage(const char* filename, UIController* uiController, int animationRows, int animationTime, bool loop);
	virtual ~UIDynamicImage();

	// Cambia la textura de la imagen. Si habia una textura anteriormente, se eliminara.
	void setTexture(Texture* texture);
};