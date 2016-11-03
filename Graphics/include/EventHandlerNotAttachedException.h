/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class EventHandlerNotAttachedException : public Exception{

public:
	EventHandlerNotAttachedException() : Exception("Se intento acceder a un elemento que no tiene funcion de event handler asociada"){};
	EventHandlerNotAttachedException(string message) : Exception(message){};
};