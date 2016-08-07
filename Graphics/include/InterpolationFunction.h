/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

class InterpolationFunction{

protected:
	unsigned int totalInterpolationTime;

public:
	InterpolationFunction(unsigned int totalInterpolationTime){ this->totalInterpolationTime = totalInterpolationTime; }
	virtual ~InterpolationFunction(){};

	// Devuelve un valor entre 0 y 1 representando el estado actual de la interpolacion
	virtual float getInterpolationFactor(unsigned int elapsedTime) = 0;

	void setTotalInterpolationTime(unsigned int time){ this->totalInterpolationTime = time; }
};