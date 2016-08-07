/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class NullPointerException : public Exception{

public:
	NullPointerException() : Exception("El puntero apunta a una referencia nula cuando no deberia"){};
	NullPointerException(string message) : Exception(message){};
};