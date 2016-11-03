/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Plane.h"
#include "BoundingBox.h"

class ViewFrustum{

private:
	Plane planes[6];  // Los seis planos del viewing frustum
	float ratio, angle, nearD, farD;
	float nh, nw, fh, fw;	

public:
	ViewFrustum();
	virtual ~ViewFrustum();

	void setPerspectiveParameters(float angle, float ratio, float nearD, float farD);
	void update(vec3 cameraPosition, vec3 lookAt, vec3 up, vec3 side);
	bool isVisible(BoundingBox* boundingBox);
};