/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class InitializationException : public Exception{

public:
	InitializationException() : Exception("Error en la inicializacion de un componente"){};
	InitializationException(string message) : Exception(message){};
};