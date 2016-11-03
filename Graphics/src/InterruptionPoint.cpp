/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "InterruptionPoint.h"
#include "InitializationException.h"

WindowConnector* InterruptionPoint::windowConnector = NULL;

InterruptionPoint::InterruptionPoint(){
	this->interrupted = false;
}
	
void InterruptionPoint::interrupt(){
	this->interrupted = true;

	if(windowConnector == NULL){
		throw new InitializationException("Falta setear el WindowConnector al InterruptPoint");
	}	

	// Nos quedamos en el ciclo mientras no se resuma la interrupción
	while(this->interrupted){
		windowConnector->OnUpdate();
		windowConnector->OnRender();
	}
}
	
void InterruptionPoint::resume(){
	this->interrupted = false;
}