/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <string>
#include <map>
#include "Scene.h"
using namespace std;

// Provee carga de escenas con manejo de cache (las escenes no se clonan, son una misma instancia)
class SceneCache{

private:
	static SceneCache* instance;
	map<string, Scene*> cache;

	SceneCache(){};

public:
	static SceneCache* getInstance();
	Scene* loadFromXmlFile(string filename);
};