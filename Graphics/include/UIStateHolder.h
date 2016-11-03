/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIState.h"

class UIElement;

class UIStateHolder{

private:
	//Determina si el estado actualizado
	bool updated;

	//Los posibles estados del elemento
	UIState* initialState;
	UIState* actualState;
	UIState* finalState;
	bool transitionEnabled;
	bool loopTransition;
	UIElement* callbackElement;
	int callbackId;
			
	//Control de tiempos de transiciones
	int transitionTime;
	int timeSlot;

	//Interpola entre los estados para calcular el estado actual
	void interpolateStates();
	
public:
	UIStateHolder();
	virtual ~UIStateHolder();
	//**************************************************************************
	//*                     Transiciones entre estados                         *
	//**************************************************************************
	//Va de un estado inicial a un estado final en un tiempo determinado
	void makeStateTransition(UIState* startState, UIState* endState, int time, bool loop=false);
	// Idem anterior pero al terminar con la transicion ejecuta un callback al elemento elegido, pasandole un id para saber de que callback se trata
	void makeStateTransition(UIState* startState, UIState* endState, int time, bool loop, UIElement* callbackElement, int callbackId);
	//Va del estado actual a un estado final en un tiempo determinado
	void makeStateTransition(UIState* endState, int time, bool loop=false);
	// Idem anterior pero al terminar con la transicion ejecuta un callback al elemento elegido, pasandole un id para saber de que callback se trata
	void makeStateTransition(UIState* endState, int time, bool loop, UIElement* callbackElement, int callbackId);
	//Va a un estado final instantaneamente
	void makeStateTransition(UIState* endState);

	//**************************************************************************
	//*                         Otros metodos publicos                         *
	//**************************************************************************
	//Manejan el flujo de las transiciones de estados del elemento
	void stopTransitions();
	void startTransitions();
	void pauseTransitions();

	UIState* getActualState(){return this->actualState; }

	void draw();
	void update(UIState* externalState);
	
	//Evalua si se realizo click en algun elemento del estado actual
	UIEventHandler evaluateClick(vec2 coords, int state, UIState* parentState);

	// Determina si el estado es visible o esta a punto de serlo proximamente
	bool isVisible();

	// Metodos para comparar estados iniciales y finales
	bool isTheFinalState(UIState* state){ return (this->finalState == state); }
	bool isTheInitialState(UIState* state){ return (this->initialState == state); }
};