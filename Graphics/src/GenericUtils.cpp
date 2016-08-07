/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "GenericUtils.h"
#include <stdio.h>
#include <stdlib.h>

#define UNSIGNED_INT_MAX_VALUE 65535

float GenericUtils::modulo(float valor){	
	return (valor < 0) * -valor + (1 - (valor < 0)) * valor;
}
int GenericUtils::modulo(int valor){
	return (valor < 0) * -valor + (1 - (valor < 0)) * valor;
}

unsigned int GenericUtils::generateHashCode(list<unsigned int>* values){
	unsigned int hashCode = 0;
	unsigned int i = 1;
	for(list<unsigned int>::iterator it = values->begin(); it != values->end(); it++){		
		hashCode += ((*it) * i) % UNSIGNED_INT_MAX_VALUE;
	}
	return hashCode;
}

bool GenericUtils::isValueModuleBetween(float valueToEvaluate, float minValue, float maxValue){
	valueToEvaluate = modulo(valueToEvaluate);
	return (valueToEvaluate >= minValue && valueToEvaluate < maxValue);
}

int GenericUtils::getRandomNumber(int minValue, int maxValue){	
	return minValue + rand() % (maxValue-minValue+1);
}