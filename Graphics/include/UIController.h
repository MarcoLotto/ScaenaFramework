/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIForm.h"
#include <list>
using namespace std;

class UIController{

private:
	// Guarda las forms disponibles
	list<UIForm*> forms;
	// Guarda aquellas forms a las que se le dieron foco o se agregaron durante el ultimo frame
	list<UIForm*> formsToUpdate;

	//Como es el punto de entrada se entrega un estado nulo como estado externo
	UIState* emptyState;

	//Contiene el ultimo evento del elemento en foco
	UIEventHandler onFocusEvent;
	bool holdingClick;
	bool holdingClickState;
	vec2 mouseActualCoords;
	UIForm* formOnFocus;

	vec2 screenSize, pointsToPixelsScaleFactor;
	vec2 normalizeCoords(vec2 coords);

	void manageHoldedClick();
	UIForm* reorganizeForms(std::list<UIForm*>::reverse_iterator it);

	// Agrega para renderizar y actualiza aquellos forms que tomaron foco o se agregaron durante el ultimo frame
	void updateFocusedFormsAndNewForms();
	void searchAndReorganizeForm(UIForm* form);

public:
	UIController();

	//Agrega un form nuevo
	void addForm(UIForm* form);

	list<UIForm*> getElements(){ return this->forms; }
	
	void manageClick(vec2 coords, int state);
	void manageClickRelease(vec2 coords, int state);
	void manageKeyDown(int key);
	void manageKeyUp(int key);

	void setScreenSize(vec2 size){ this->screenSize = size; }
	vec2 getScreenSize(){ return this->screenSize; }
    
    // Determina el tamaño de la pantalla en puntos tocables (en pantallas touch, puntos no siempre son lo mismo que pixels)
    void setScreenPointsSize(vec2 pointsSize){ this->pointsToPixelsScaleFactor = this->screenSize / pointsSize; }

	void setMouseActualCoords(vec2 absoluteCoords);

	//Le da foco a un form en particular. Si el form no existe lo agrega.
	void giveFocusTo(UIForm* form);

	// Devuelve el form en foco (o NULL de no haber ninguno)
	UIForm* getFormOnFocus(){ return this->formOnFocus; }

	void draw();
	void update();
};