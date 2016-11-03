/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class InvalidSubroutineNameException : public Exception{

public:
	InvalidSubroutineNameException() : Exception("El nombre de la subrutina no existe en el shader"){};
	InvalidSubroutineNameException(string name) : Exception("No existe la subrutina " + name){};
};