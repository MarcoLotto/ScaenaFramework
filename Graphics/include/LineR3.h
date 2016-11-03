/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <glm/glm.hpp>
typedef glm::vec3 vec3;

class LineR3{

private:
	// Datos de la recta ( (x, y, z) = x0 + h*u ) 
	vec3 u, x0;

public:
	LineR3(vec3 p1, vec3 p2);	

	// Devuelve el punto de la recta en R3 correspondiente al step indicado
	vec3 getPoint(float step);

	// Identifica si un punto de la recta esta entre otros de la recta. Recordar, los puntos deben estar contenidos en la recta.
	bool isPointBetweenTwoOthers(vec3 pointInBetween, vec3 p1, vec3 p2);

	// Devuelve la proyeccion ortogonal (punto mas cercano) de un punto sobre la recta
	vec3 getOrthogonalProyectedPoint(vec3 pointOutsideLine); 

	// Devuelve la distancia minima de un punto hasta la recta
	float getMinimalDistanceToPoint(vec3 pointOutsideLine);
};