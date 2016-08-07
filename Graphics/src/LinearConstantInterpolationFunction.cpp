/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "LinearConstantInterpolationFunction.h"

float LinearConstantInterpolationFunction::getInterpolationFactor(unsigned int elapsedTime){
	return (1.0f - (this->totalInterpolationTime - elapsedTime) / (float)this->totalInterpolationTime);
}