/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class AnimationLoadException : public Exception{

public:
	AnimationLoadException() : Exception("Error durante la carga de una animacion"){};
	AnimationLoadException(string message) : Exception(message){};
};