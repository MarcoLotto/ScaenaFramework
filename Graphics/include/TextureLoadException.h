/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class TextureLoadException : public Exception{

public:
	TextureLoadException() : Exception("Error en la carga de textura"){};
	TextureLoadException(string message) : Exception(message){};
};