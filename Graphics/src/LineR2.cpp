/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "LineR2.h"
#include "GenericUtils.h"

LineR2::LineR2(vec2 p1, vec2 p2){
	this->u = glm::normalize(p2 - p1);
	this->x0 = p1;
}

vec2 LineR2::getPoint(float step){
	return (this->x0 + step * this->u);
}

vec2 LineR2::getOrthogonalProyectedPoint(vec2 pointOutsideLine){
	float scalarProduct = (this->u.x * pointOutsideLine.x + this->u.y * pointOutsideLine.y);	
	return scalarProduct * this->u;
}

// Consigue la distancia al punto de la recta mas cercano a otro punto dado
float LineR2::getMinimalDistanceToPoint(vec2 pointOutsideLine){
	vec2 proyectedPoint = this->getOrthogonalProyectedPoint(pointOutsideLine);
	return glm::distance(proyectedPoint, pointOutsideLine);
}

// Determina si esta linea es paralela con otra
bool LineR2::isParallel(LineR2* otherLine){
	return (this->u == otherLine->getU());
}

// Determina el punto de interseccion entre dos rectas
vec2 LineR2::getIntersectionPoint(LineR2* otherLine){
	vec2 ua = this->getU();
	vec2 ub = otherLine->getU();
	vec2 xa = this->getX0();
	vec2 xb = otherLine->getX0();

	float hb = ((xb.x - xa.x)/ua.x - (xb.y - xa.y)/ua.y) / (ub.y/ua.y - ub.x/ua.x);
	return vec2(xb.x + hb * ub.x, xb.y + hb * ub.y);
}

bool LineR2::isPointBetweenTwoOthers(vec2 pointInBetween, vec2 p1, vec2 p2){	
	float p1Length = glm::distance(this->x0, p1);
	float p2Length = glm::distance(this->x0, p2);
	float pointInBetweenLength = glm::distance(this->x0, pointInBetween);
	return ((p1Length <= pointInBetweenLength) && (p2Length >= pointInBetweenLength)) || ((p2Length <= pointInBetweenLength) && (p1Length >= pointInBetweenLength));
}