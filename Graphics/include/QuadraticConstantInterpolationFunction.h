/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "LinearConstantInterpolationFunction.h"

class TimeParameterInterpolator;

class QuadraticConstantInterpolationFunction : public InterpolationFunction{

private:

public:
	QuadraticConstantInterpolationFunction(unsigned int totalInterpolationTime);
	virtual ~QuadraticConstantInterpolationFunction();

	// Devuelve un valor entre 0 y 1 representando el estado actual de la interpolacion
	float getInterpolationFactor(unsigned int elapsedTime);
};



