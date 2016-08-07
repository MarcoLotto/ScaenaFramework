/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class PropertyCannotBeAcessedException : public Exception{

public:
	PropertyCannotBeAcessedException() : Exception("No se puede tener acceso a la propiedad requerida"){};
	PropertyCannotBeAcessedException(string message) : Exception("No se puede acceder a la propiedad " + message){};
};