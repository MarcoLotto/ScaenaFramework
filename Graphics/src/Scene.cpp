/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "Scene.h"

Scene::Scene(){
	this->objectManager = new ObjectManager();
	this->lightingManager = new LightingManager(this->objectManager);
	this->currentCamera = NULL;
	this->name = "";
}

void Scene::setLightingManager(LightingManager* lightingManager){
	this->lightingManager = lightingManager;
}
LightingManager* Scene::getLightingManager(){
	return this->lightingManager;
}

void Scene::setObjectManager(ObjectManager* objectManager){
	this->objectManager = objectManager;
}
ObjectManager* Scene::getObjectManager(){
	return this->objectManager;
}


void Scene::update(){
	this->objectManager->update();
}
void Scene::render(Camara* camera){
	this->objectManager->render(camera);
}

void Scene::renderFromCurrentCamera(){
	this->render(this->currentCamera);
}

void Scene::setCurrentCamera(Camara* camera){
	this->currentCamera = camera;
}
	
// Elimina tanto luces como objetos (no los borra fisicamente)
void Scene::clear(){
	this->objectManager->clear();
	this->lightingManager->clear();
}

Scene* Scene::clone(){
	Scene* clone = new Scene();
	clone->setCurrentCamera(this->currentCamera);
	clone->setName(this->name);

	// No voy a precisar los objectmanager y lightingmanager que el scene trae por defecto, los borro
	delete clone->getObjectManager();
	delete clone->getLightingManager();

	// Ahora clono los actuales managers y los asigno
	LightingManager* cloneLightingManager = this->getLightingManager()->clone(NULL);
	ObjectManager* cloneObjectManager = this->getObjectManager()->clone(cloneLightingManager);
	cloneLightingManager->getShadowManager()->setObjectManager(cloneObjectManager);
	clone->setLightingManager(cloneLightingManager);
	clone->setObjectManager(cloneObjectManager);

	return clone;
}

Scene::~Scene(){
	if(this->objectManager){
		delete this->objectManager;
	}
	if(this->lightingManager){
		delete this->lightingManager;
	}
}