/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "InterpolationFunction.h"

class LinearConstantInterpolationFunction : public InterpolationFunction{

public:
	LinearConstantInterpolationFunction(unsigned int totalInterpolationTime) : InterpolationFunction(totalInterpolationTime) {}

	// Devuelve un valor entre 0 y 1 representando el estado actual de la interpolacion
	float getInterpolationFactor(unsigned int elapsedTime);
};

