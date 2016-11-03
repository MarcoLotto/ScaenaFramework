/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class FrameBufferWriteLockViolationException : public Exception{

public:
	FrameBufferWriteLockViolationException() : Exception("Se intento hacer un bind a un frame buffer con writelock"){};
	FrameBufferWriteLockViolationException(string message) : Exception(message){};
};