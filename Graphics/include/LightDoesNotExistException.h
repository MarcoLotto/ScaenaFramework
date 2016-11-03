/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class LightDoesNotExistException : public Exception{

public:
	LightDoesNotExistException() : Exception("El handler suministrado no esta asociado a ninguna luz"){};
	LightDoesNotExistException(string message) : Exception(message){};
};