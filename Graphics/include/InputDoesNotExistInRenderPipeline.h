/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class InputDoesNotExistInRenderPipeline : public Exception{

public:
	InputDoesNotExistInRenderPipeline() : Exception("Un input del render stage no existe en el pipeline."){};
	InputDoesNotExistInRenderPipeline(string message) : Exception(message){};
};