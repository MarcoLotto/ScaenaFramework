/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class FileProcessingException : public Exception{

public:
	FileProcessingException() : Exception("No hay API bindeado"){};
	FileProcessingException(string message) : Exception(message){};
};