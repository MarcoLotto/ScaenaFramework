/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class ApiVariableTypeDoesNotExistException : public Exception{

public:
	ApiVariableTypeDoesNotExistException() : Exception("Falta declarar el tipo de variable o no existe."){};
	ApiVariableTypeDoesNotExistException(string message) : Exception(message){};
};