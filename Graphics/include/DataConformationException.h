/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class DataConformationException : public Exception{

public:
	DataConformationException() : Exception("La data esta mal conformada"){};
	DataConformationException(string message) : Exception(message){};
};