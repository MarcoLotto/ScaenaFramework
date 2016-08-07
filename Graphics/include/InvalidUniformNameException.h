/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class InvalidUniformNameException : public Exception{

public:
	InvalidUniformNameException() : Exception("El nombre del uniform no existe en el shader"){};
	InvalidUniformNameException(string name) : Exception("No existe el uniform " + name){};
};