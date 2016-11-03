/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "WindowConnector.h"

/**
Define un punto de interrupción el cual cuando el programa llega hasta el mismo, no continua hasta que no se llame a 
resume(). Durante la interrupción se mantiene el renderizado y los updates de UI.
**/
class InterruptionPoint{
	
private:
	bool interrupted; 
	static WindowConnector* windowConnector;
	
public:
	// Debe setearse siempre quien es el manejador del onRender y el OnUpdate
	static void setWindowConnector(WindowConnector* connector){ windowConnector = connector; }

	// De no pasarse parametros, será necesario reanudar el programa utilizando resume() (desde otro thread)
	InterruptionPoint();
	
	// Comienza la interrupción (manteniendo el renderizado y update de UI)
	void interrupt();
	
	// Frena con la interrupción y continua con el programa
	void resume();
};