/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "InterpolationFunction.h"

class TimeParameterInterpolator{

private:
	unsigned int timeSlot;
	float minValue;
	float maxValue;
	unsigned int interpolationTime;
	bool interpolationStop;
	bool animatingForward;
	bool inverseAnimationEnabled;
	bool continiumAnimationEnabled;
	InterpolationFunction* actualInterpolationFunction;
	
	unsigned int getElapsedTime();
	bool getAnimationDirection();
	bool processContiniumAnimation();
	bool processInverseAnimation();
	float getInterpolationFactor();	
	
public:
	TimeParameterInterpolator(float minValue, float maxValue, unsigned int interpolationTime);
	virtual ~TimeParameterInterpolator();

	// Devuelve el valor actual del factor
	float getCurrentValue();

	// El valor minimo y maximo que puede tomar el valor, y cuanto tiempo durara esa interpolacion
	void setMinValue(float value){ this->minValue = value; }
	void setMaxValue(float value){ this->maxValue = value; }
	void setInterpolationTime(unsigned int time);

	// Enciende o apaga la interpolacion del factor. Si esta en stop, siempre se obtiene el valor maximo.
	void setPlay(){ this->interpolationStop = false; }
	void setStop(){ this->interpolationStop = true; }

	// Vuelve el tiempo a cero
	void reset();
	
	// Define si la animacion esta prendida o apagada
	bool isStopped(){ return this->interpolationStop; }

	// Define si al animar en loop, se volvera a empezar desde cero, o bien se ira en sentido inverso
	void setInverseAnimationEnabled(bool enabled){ this->inverseAnimationEnabled = enabled; this->continiumAnimationEnabled = false;}

	// Define si al terminar de animar (al llegar al valor maximo o minimo) se debe continuar desde ese punto con la misma velocidad
	// Ej. minVal=0, maxVal=30 -> termina animacion -> minVal=30, maxVal=60 -> se resetea el tiempo a cero (tambien funciona en sentido inverso)
	void setContiniumAnimationEnabled(bool enabled){ this->continiumAnimationEnabled = enabled; this->inverseAnimationEnabled = false;}

	// Seleccion de tipo de funcion de interpolacion
	void selectLinearConstantInterpolation();
	void selectLinearNaturalInterpolation();
};