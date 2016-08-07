/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "LineR3.h"
#include "GenericUtils.h"

LineR3::LineR3(vec3 p1, vec3 p2){
	this->u = glm::normalize(p2 - p1);
	this->x0 = p1;
}

vec3 LineR3::getPoint(float step){
	return (this->x0 + step * this->u);
}

bool LineR3::isPointBetweenTwoOthers(vec3 pointInBetween, vec3 p1, vec3 p2){	
	float p1Length = glm::distance(this->x0, p1);
	float p2Length = glm::distance(this->x0, p2);
	float pointInBetweenLength = glm::distance(this->x0, pointInBetween);
	return ((p1Length <= pointInBetweenLength) && (p2Length >= pointInBetweenLength)) || ((p2Length <= pointInBetweenLength) && (p1Length >= pointInBetweenLength));
}

vec3 LineR3::getOrthogonalProyectedPoint(vec3 pointOutsideLine){
	vec3 pointToX0 = pointOutsideLine - this->x0;
	float scalarProduct = (this->u.x * pointToX0.x + this->u.y * pointToX0.y + this->u.z * pointToX0.z);	
	return this->x0 + scalarProduct * this->u;
}

// Consigue la distancia al punto de la recta mas cercano a otro punto dado
float LineR3::getMinimalDistanceToPoint(vec3 pointOutsideLine){
	vec3 proyectedPoint = this->getOrthogonalProyectedPoint(pointOutsideLine);
	return glm::distance(proyectedPoint, pointOutsideLine);
}