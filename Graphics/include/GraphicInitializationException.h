/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class GraphicInitializationException : public Exception{

public:
	GraphicInitializationException() : Exception("El API ha fallado al iniciarse"){};
	GraphicInitializationException(string message) : Exception(message){};
};