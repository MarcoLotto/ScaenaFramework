/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <list>
using namespace std;

class GenericUtils {

public:
	static float modulo(float valor);
	static int modulo(int valor);
	static unsigned int generateHashCode(list<unsigned int>* values);

	static int getRandomNumber(int minValue, int maxValue);

	// Indica si el modulo del valor dado esta entre los valores [minValue, maxValue).
	static bool isValueModuleBetween(float valueToEvaluate, float minValue, float maxValue);
};