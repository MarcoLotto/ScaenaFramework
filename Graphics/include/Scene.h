/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "LightingManager.h"
#include "ObjectManager.h"
#include "Camara.h"

class Scene{

private:
	string name;
	LightingManager* lightingManager;
	ObjectManager* objectManager;
	Camara* currentCamera;
public:
	Scene();
	virtual ~Scene();

	void setLightingManager(LightingManager* lightingManager);
	LightingManager* getLightingManager();

	void setObjectManager(ObjectManager* objectManager);
	ObjectManager* getObjectManager();
	
	void update();
	
	void render(Camara* camera);
	void renderFromCurrentCamera();

	void setCurrentCamera(Camara* camera);
	Camara* getCurrentCamera(){ return this->currentCamera; }

	// Clona la escena actual, junto con su lightingmanager y objectmanager
	Scene* clone();

	// Permite definir un nombre para la escena
	string getName(){ return this->name; }
	void setName(string sceneName){ this->name =sceneName; }

	// Elimina tanto luces como objetos (no los borra fisicamente)
	void clear();
};