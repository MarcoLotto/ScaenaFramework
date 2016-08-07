/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIElement.h"
#include "UICheckBox.h"
#include <list>
#include <vector>
using namespace std;

class UIScrollBar;

class UIList : public UIElement{

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

	void initializeButtonItems(int displayElementsCount, float listHeight);

	//Estados
	UIState* basicState;
	UIState* withScrollBarState;

	//Elementos
	vector<UIElement*>* items;
	list<UICheckBox*>* buttonItems;
	UIScrollBar* scrollbar;

	//Otros atributos
	const char* filename;
	int displayElementsCount;
	UIElement* selectedElement;
	int selectedElementId;
	float listHeight;
	UICheckBox* selectedButton;
	
	//Auxiliares para procesamientos de eventos
	void findClickedElementEvent();

public:
	UIList(const char* filename, int displayElementsCount, float listHeight, UIController* uiController);
	virtual ~UIList();

	void draw();
	void update(UIState* externalState);

	void addElement(UIElement* element);

	void setScrollBarPositionX(float value);

	//Borra todos los elementos de la lista BORRANDO TAMBIEN LOS ELEMENTOS CONTENIDOS EN LA MISMA
	void clear();

	//Devuelve el elemento seleccionado actualmente
	UIElement* getSelctedElement(){ return this->selectedElement; }
	//Devuelve la posicion en la lista del elemento seleccionado actualmente
	int getSelectedElementById(){ return this->selectedElementId; }

	//Setea el elemento activo, por su posicion o id en la lista (desde 0). Si sobrepasa el largo de la lista no selecciona nada. 
	void setSelectedElementById(int position);

};