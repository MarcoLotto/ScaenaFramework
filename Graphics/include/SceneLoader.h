/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Scene.h"

class SceneLoader{

private:

public:
	static Scene* loadFromXmlFile(const char* filename);
};