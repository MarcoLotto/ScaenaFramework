/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Exception.h"

class FeatureNotAvaibleException : public Exception{

public:
	FeatureNotAvaibleException() : Exception("La caracteristica solicitada no esta disponible para su hardware y driver actuales"){};
	FeatureNotAvaibleException(string message) : Exception(message){};
};