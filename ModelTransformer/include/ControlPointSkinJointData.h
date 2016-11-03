/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "SkeletonDataTransfer.h"

class ControlPointSkinJointData{

public:
	SkeletonDataTransfer* skeletonJointIndex;   // Identifica a que bone del skeleton corresponde este peso de joint
	float jointToVertexWeight;					// Identifica el peso que este joint tiene sobre el vertice

	ControlPointSkinJointData(){ this->skeletonJointIndex = NULL; jointToVertexWeight = 0.0f; }
};