/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIForm.h"
#include "UIStaticImage.h"
#include "UIButton.h"
#include <vector>
using namespace std;

class GameLifeManager;

class MenuStatusBarForm : public UIForm{

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
    
    void evaluateBackButtonDisplay();

	//Estados
	UIState* state1;
    UIState* noBackState;
		
	//Agrego los subelementos
	UIStaticImage* upperBar;
	UiButton* button1;

	// Se definen los manejadores de eventos
	void processBackButtonEvent();

	// Me guardo las pantallas anteriores para poder volver atras
	vector<UIForm*> previousForms;
	UIForm* actualForm;

public:
	MenuStatusBarForm(UIController* uiController);
	virtual ~MenuStatusBarForm();

	// Agrega un registro de stack para acordarse a donde volver al tocar back.
	void addFormStackRegister(UIForm* actualForm, UIForm* nextForm);

	// Limpia el stack de pantallas
	void clearStackRegister();

	// Actualiza todos los indicadores de la barra (como dinero, mensajes, etc)
	void updateBarDisplayData();
};