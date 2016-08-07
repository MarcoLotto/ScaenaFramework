/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <glm/glm.hpp>
#include "UIStateHolder.h"
#include "UIFactory.h"
#include "Callbackable.h"
#include <list>
using namespace std;

class UIState;
class UIController;

typedef glm::vec2 vec2;

//*********************************************************************************
//* El corazon de la UI. De esta heredan todos los componentes. Contiene estados. *
//*********************************************************************************
class UIElement : public Callbackable{

protected:
	//Conocimiento del uiController para poder pedir y crear elementos externos
	UIController* uiController;

	//Los estados actuales del elemento
	UIStateHolder* internalState;
	UIStateHolder* externalState;

	//Contiene el ultimo evento del elemento en foco
	UIEventHandler onFocusEvent;

	UIState mergeParameters(UIState* parentState);
		
	//Borra estados y subelementos
	void deleteElement(UIElement* element);
	void deleteState(UIState* state);

	//Maneja los eventos que se producen cuando un elemento consigue el foco
	virtual void handleEvents() = 0;

private:
	//Identifica si se necesita o no mergear el estado del padre
	bool parentMergeAllowed;
	
	//**************************************************************************
	//*                        Metodos virtuales privados                      *
	//**************************************************************************
	//Incializacion de los estados del elemento 
	virtual void initiateStates() = 0;
	//Inicializacion de los subelementos
	virtual void initiateElements() = 0;
	//Linkea los elementos con los estados (cuales deben ser dibujados en cada estado)
	virtual void bindElementsToStates() = 0;

	//**************************************************************************
	//*                         Otros metodos privados                         *
	//**************************************************************************
	//Realiza la interpolacion entre estados
	void calculateActualState();
		
	//Dado un punto de la pantalla, le aplica las transformaciones del elemento actual
	vec2 getFixedCoordPoint(vec2 coords);	
	
public:	
	UIElement(UIController* uiController);
	virtual ~UIElement();
	//**************************************************************************
	//*                       Metodos virtuales publicos                       *
	//**************************************************************************
	virtual void draw();
	virtual void update(UIState* externalState);

	virtual UIEventHandler onClick(vec2 coords, int state);
	virtual UIEventHandler onClickHold(vec2 coords, int state);
	virtual UIEventHandler onClickRelease(vec2 coords, int state);

	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);	

	// Permite que las acciones (como el finalizar un cambio de estado) le envien eventos para indicar esto
	virtual void onCallback(int callbackId){}
	
	//**************************************************************************
	//*                         Otros metodos publicos                         *
	//**************************************************************************
	//Realiza la inicializacion del elemento llamando a la creacion de estados y elementos  
	void initiate();

	//Manejan el flujo de las transiciones de estados del elemento
	void stopTransitions();
	void startTransitions();
	void pauseTransitions();	

	// Manda a manejar todos los eventos que se producen en este elemento o en sus hijos
	void handleElementEvents();

	//Accesors para el estado externo del elemento
	virtual UIStateHolder* getExternalState(){ return this->externalState; }

	void setParentMergeAllowed(bool value){ this->parentMergeAllowed = value; }
	bool isParentMergeAllowed(){ return this->parentMergeAllowed; }

	UIEventHandler* getOnFocusEvent(){ return (&this->onFocusEvent); }

	void setUiController(UIController* controller){ this->uiController = controller; }
	UIController* getUiController(){ return this->uiController; }
};