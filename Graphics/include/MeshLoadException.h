/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class MeshLoadException : public Exception{

public:
	MeshLoadException() : Exception("Error durante la carga de un mesh"){};
	MeshLoadException(string message) : Exception(message){};
};