/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class RenderStageNotAssignedInPipeline : public Exception{

public:
	RenderStageNotAssignedInPipeline() : Exception("Falta asociar un render stage para el pipeline elegido"){};
	RenderStageNotAssignedInPipeline(string message) : Exception(message){};
};