/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <glm/glm.hpp>
typedef glm::vec3 vec3;

class SensorsData{

private:
	vec3 acelerometer;

public:
	// Coordenadas del acelerometro
	vec3 getAcelerometer(){ return this->acelerometer; }
	void setAcelerometer(vec3 acelerometerData){ this->acelerometer = acelerometerData; }
};