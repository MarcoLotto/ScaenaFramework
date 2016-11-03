/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <stdexcept>
#include <string>
#include "Logger.h"
using namespace std;

class Exception : public std::runtime_error{

public:
	Exception(string message) : std::runtime_error(message){
		Logger::getInstance()->logError(new Log("Exception: " + message));
	}
};
