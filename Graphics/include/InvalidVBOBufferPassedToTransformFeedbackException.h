/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class InvalidVBOBufferPassedToTransformFeedbackException : public Exception{

public:
	InvalidVBOBufferPassedToTransformFeedbackException() : Exception("El VBO buffer pasado al shader de transform feedback no es correcto"){};
	InvalidVBOBufferPassedToTransformFeedbackException(string message) : Exception(message){};
};