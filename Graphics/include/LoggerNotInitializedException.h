/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class LoggerNotInitializedException : public Exception{

public:
	LoggerNotInitializedException() : Exception("Falta inicializar el logger antes de usarlo"){};
	LoggerNotInitializedException(string message) : Exception(message){};
};