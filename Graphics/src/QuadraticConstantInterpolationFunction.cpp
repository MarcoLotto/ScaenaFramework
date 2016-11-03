/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "QuadraticConstantInterpolationFunction.h"

QuadraticConstantInterpolationFunction::QuadraticConstantInterpolationFunction(unsigned int totalInterpolationTime) : InterpolationFunction(totalInterpolationTime){ 
}

QuadraticConstantInterpolationFunction::~QuadraticConstantInterpolationFunction(){
}

float QuadraticConstantInterpolationFunction::getInterpolationFactor(unsigned int elapsedTime){	
	float linearFactor = elapsedTime / (float) this->totalInterpolationTime;
	return linearFactor * linearFactor;
}