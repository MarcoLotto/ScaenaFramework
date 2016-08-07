/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class ApiNotBindException : public Exception{

public:
	ApiNotBindException() : Exception("No hay API bindeado"){};
	ApiNotBindException(string message) : Exception(message){};
};