/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ViewFrustum.h"

#define TOP 0
#define BOTTOM 1
#define LEFT 2
#define RIGHT 3
#define NEAR 4
#define FAR 5

ViewFrustum::ViewFrustum(){
}
ViewFrustum::~ViewFrustum(){
}

void ViewFrustum::setPerspectiveParameters(float angle, float ratio, float nearD, float farD) {
	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	// Se arma los parametros de los planos near y far
	float tang = (float) glm::tan(angle) ;
	this->nh = nearD * tang;
	this->nw = nh * ratio;
	this->fh = farD * tang;
	this->fw = fh * ratio;
}

void ViewFrustum::update(vec3 cameraPosition, vec3 lookAt, vec3 up, vec3 side) {
	// compute the centers of the near and far planes	
	vec3 nc,fc;
	nc = cameraPosition + lookAt * this->nearD;
	fc = cameraPosition + lookAt * this->farD;

	this->planes[NEAR].constructFromPointNormal(nc, lookAt);
	this->planes[FAR].constructFromPointNormal(fc, -lookAt);

	vec3 aux,normal;
	aux = glm::normalize((nc + up*nh) - cameraPosition);	
	normal = glm::cross(side, aux);
	this->planes[TOP].constructFromPointNormal(nc + up*nh, normal);

	aux = glm::normalize((nc - up*nh) - cameraPosition);
	normal = glm::cross(aux, side);
	this->planes[BOTTOM].constructFromPointNormal(nc - up*nh, normal);

	aux = glm::normalize((nc - side*nw) - cameraPosition);
	normal = glm::cross(up, aux);
	this->planes[LEFT].constructFromPointNormal(nc - side*nw, normal);

	aux = glm::normalize((nc + side*nw) - cameraPosition);
	normal = glm::cross(aux, up);
	this->planes[RIGHT].constructFromPointNormal(nc + side*nw, normal);
}

bool ViewFrustum::isVisible(BoundingBox* boundingBox){
	// Transformo el cubo en una esfera para acelerar la comparación
	vec3 centerPoint = (boundingBox->getAbsoluteMaxPoint() + boundingBox->getAbsoluteMinPoint()) / 2.0f;
	float radius = glm::length(boundingBox->getAbsoluteMaxPoint() - boundingBox->getAbsoluteMinPoint()) / 2.0f;
	
	// Ahora me fijo si el punto está dentro de los 6 planos del viewing frustum
	for(unsigned int i=0; i < 6; i++){
		if(this->planes[i].signedDistance(centerPoint) + radius < 0){
			return false;
		}
	}
	return true;
}

