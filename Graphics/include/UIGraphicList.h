/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIElement.h"
#include "UiButton.h"
#include "UIGraphicListItem.h"

class UIGraphicList : public UIElement{

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
	// ESte update se utiliza para cuando hay un cambio (como al tocar up o down en al lista) para hacer refresh a los botones
	void updateGraphicList();
	
	void graphicListInitialize(UIController* uiController, vec2 scrollBarSize, int scrollBarOptionsCount, string upButtonTexture, string downButtonTexture, 
		string backgroundTexture, vec2 labelPos, float labelSize, vec2 imgPos, vec2 imgSize);

	//Estados
	UIState* basicState;
	
	//Elementos
	UiButton* upButton;
	UiButton* downButton;
	vector<UiButton*> listButtons;

	vector<UIGraphicListItem> listItems;

	// Otros atributos
	int scrollBarOptionsCount;
	string upButtonTexture, downButtonTexture, backgroundTexture;
	vec2 scrollBarSize;
	int firstItemInListPosition;  // Indica a partir de que item se empieza a mostrar en el UIGraphicList
	int selectedItemPosition;     // Indica cual es la posicion del ultimo item seleccionado
	vec2 imgPos, imgSize, labelPos;
	float labelSize;
	bool usingLabels;
	
	//Auxiliares para procesamientos de eventos
	void processUpScrollButtonEvent();
	void processDownScrollButtonEvent();
	void processOptionButtonEvent(int position);

public:
	UIGraphicList(UIController* uiController, vec2 scrollBarSize, int scrollBarOptionsCount, string upButtonTexture, string downButtonTexture, 
		string backgroundTexture, vec2 labelPos, float labelSize, vec2 imgPos, vec2 imgSize);

	UIGraphicList(UIController* uiController, vec2 scrollBarSize, int scrollBarOptionsCount, string upButtonTexture, string downButtonTexture, 
		string backgroundTexture, vec2 imgPos, vec2 imgSize);

	virtual ~UIGraphicList();

	// Setea la lista de items total a mostrar en el GraphicList
	void setListItems(vector<UIGraphicListItem> items);

	// Devuelve la posicion en la lista de items del ultimo item seleccionado (o -1 si el ultimo fue up o down o bien no se selecciono nunca ninguno)
	int getSelectedItemPosition(){ return this->selectedItemPosition; }

	// Limpia la lista, borrando todos los items y eliminando fisicamente sus texturas
	void clear();
};
