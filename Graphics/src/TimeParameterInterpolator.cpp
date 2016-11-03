/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "TimeParameterInterpolator.h"
#include "TimeManager.h"
#include "LinearConstantInterpolationFunction.h"
#include "QuadraticConstantInterpolationFunction.h"

TimeParameterInterpolator::TimeParameterInterpolator(float minValue, float maxValue, unsigned int interpolationTime){
	this->minValue = minValue;
	this->maxValue = maxValue;
	this->interpolationTime = interpolationTime;
	this->timeSlot = TimeManager::getInstance()->reservTimeSlot();
	this->interpolationStop = true;
	this->animatingForward = true;
	this->inverseAnimationEnabled = true;
	this->continiumAnimationEnabled = false;
	this->actualInterpolationFunction = new LinearConstantInterpolationFunction(interpolationTime);	
}

TimeParameterInterpolator::~TimeParameterInterpolator(){
	TimeManager::getInstance()->removeTimeSlot(this->timeSlot);
	delete actualInterpolationFunction;
}

float TimeParameterInterpolator::getCurrentValue(){
	// Si la animacion esta en stop, retorno si	empre el valor maximo
	if(this->interpolationStop){ 
		return this->maxValue;
	}
	//Verifico si debo volver a cero al terminar la animacion, o hacer la animacion inversa
	bool animationDirectionIsForward = this->getAnimationDirection();

	// Realizo una interpolacion lineal del valor (dependiendo del sentido lo proceso diferente)
	float lerpValue = this->getInterpolationFactor();
	if(animationDirectionIsForward)
		return this->minValue + (this->maxValue - this->minValue) * lerpValue;
	return this->minValue + (this->maxValue - this->minValue) * (1.0f - lerpValue);
}

float TimeParameterInterpolator::getInterpolationFactor(){
	// Consigo el tiempo que paso, si ya llegué al limite de tiempo, vuelvo al principio
	unsigned int elapsedTime = this->getElapsedTime();

	// Interpolamos basandonos en la funcion de interpolacion		
	return this->actualInterpolationFunction->getInterpolationFactor(elapsedTime);		
}

unsigned int TimeParameterInterpolator::getElapsedTime(){
	TimeManager* timeManager = TimeManager::getInstance();
	unsigned int elapsedTime = timeManager->getElapsedTime(this->timeSlot);
	if(elapsedTime > this->interpolationTime){
		bool hasToResetTime = this->processInverseAnimation();
		hasToResetTime += this->processContiniumAnimation();
		if(hasToResetTime){
			// Reseteo el tiempo
			timeManager->updateTimeSlot(this->timeSlot);
			elapsedTime = timeManager->getElapsedTime(this->timeSlot);
		}
		else{
			elapsedTime = this->interpolationTime;
		}
	}
	return elapsedTime;
}

bool TimeParameterInterpolator::processInverseAnimation(){
	if(this->inverseAnimationEnabled){
		// Invierto el sentido de loop
		this->animatingForward = !this->animatingForward;
		return true;
	}
	return false;
}

// Si esta habilitado el continium animation, si llegue al valor maximo, seteo un nuevo valor maximo para seguir la animacion en el mismo sentido y velocidad
bool TimeParameterInterpolator::processContiniumAnimation(){
	if(this->continiumAnimationEnabled){	
		float auxMinValue = this->minValue;
		this->minValue = this->maxValue;
		this->maxValue += this->maxValue - auxMinValue;
		return true;
	}	
	return false;
}

bool TimeParameterInterpolator::getAnimationDirection(){
	// Si está habilitado que se debe invertir el sentido del loop, devuelvo el sentido. Sino devuelvo hacia adelante.
	if(this->inverseAnimationEnabled){
		return this->animatingForward;
	}
	return true;
}

// Vuelve el tiempo a cero
void TimeParameterInterpolator::reset(){
	TimeManager::getInstance()->updateTimeSlot(this->timeSlot);
}

void TimeParameterInterpolator::setInterpolationTime(unsigned int time){ 
	this->interpolationTime = time; 
	this->actualInterpolationFunction->setTotalInterpolationTime(time);
}

void TimeParameterInterpolator::selectLinearConstantInterpolation(){
	delete this->actualInterpolationFunction;
	this->actualInterpolationFunction = new LinearConstantInterpolationFunction(this->interpolationTime);	
}
void TimeParameterInterpolator::selectLinearNaturalInterpolation(){
	delete this->actualInterpolationFunction;
	this->actualInterpolationFunction = new QuadraticConstantInterpolationFunction(this->interpolationTime);	
}