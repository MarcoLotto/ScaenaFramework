/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Mesh.h"

class RectangleMeshFactory{

public:
	static Mesh* createCenteredRectangleMesh();
	static Mesh* createUIRectangleMesh();
	
private:
	static bool centeredRectangleLoaded;
	static bool uiRectangleLoaded;
	static float positionData[];
	static float sidePositionData[];
	static float texturaDefaultData[];
	static float sideTextureDefaultData[];
	static unsigned int indexData[];
};
