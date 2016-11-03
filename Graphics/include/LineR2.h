/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "glm/glm.hpp"
typedef glm::vec2 vec2;

class LineR2{

private:
	// Datos de la recta ( (x, y, z) = x0 + h*u ) 
	vec2 u, x0;

public:
	LineR2(vec2 p1, vec2 p2);

	// Los parametros de vector director y un punto de la recta
	vec2 getU(){ return this->u; }
	vec2 getX0(){ return this->x0; }

	// Devuelve el punto de la recta en R3 correspondiente al step indicado
	vec2 getPoint(float step);

	// Identifica si un punto de la recta esta entre otros de la recta. Recordar, los puntos deben estar contenidos en la recta.
	bool isPointBetweenTwoOthers(vec2 pointInBetween, vec2 p1, vec2 p2);

	// Devuelve la proyeccion ortogonal (punto mas cercano) de un punto sobre la recta
	vec2 getOrthogonalProyectedPoint(vec2 pointOutsideLine); 

	// Devuelve la distancia minima de un punto hasta la recta
	float getMinimalDistanceToPoint(vec2 pointOutsideLine);

	// Determina si esta linea es paralela con otra
	bool isParallel(LineR2* otherLine);

	// Determina el punto de interseccion entre dos rectas
	vec2 getIntersectionPoint(LineR2* otherLine);
};