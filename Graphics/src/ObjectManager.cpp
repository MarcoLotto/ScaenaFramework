/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "LightingManager.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager(){
	this->posicionesLibres = 0;
	this->lightingManager = NULL;
}

int ObjectManager::addObject(Object* object){
	// Primero agrego todas las luces del objeto al lighting manager
	vector<ObjectLight*>* objectLights = object->getAttachedLights();
	for(unsigned int i=0; i < objectLights->size(); i++){
		ObjectLight* light = objectLights->at(i);
		this->lightingManager->addLight(light);
		this->lightingManager->castShadow(light->getShadowData()->isActive(), light);
	}
	//Ahora agrego el objeto al manager, si no hay posiciones libres, lo pongo al final del vector
	if(this->posicionesLibres == 0){
		this->objectContainer.push_back(object);
		return this->objectContainer.size() - 1;
	}
	//Si hay posiciones libres busco la posicion libre y lo inserto ahi
	int positionToWrite = this->getPositionByObject(NULL);
	this->objectContainer[positionToWrite] = object;
	this->posicionesLibres--;
	return positionToWrite;
}

Object* ObjectManager::getObjectByPosition(int positionInContainer){
	if(positionInContainer < this->objectContainer.size())
		return this->objectContainer[positionInContainer];
	return NULL;
}

int ObjectManager::getPositionByObject(Object* object){
	for(int i=0; i < this->objectContainer.size(); i++){
		if(this->objectContainer[i] == object)
			return i;
	}
	return -1;
}

int ObjectManager::getPositionByObjectId(string id){
	for(int i=0; i < this->objectContainer.size(); i++){
		if(this->objectContainer[i]->getId().compare(id) == 0)
			return i;
	}
	return -1;
}

Object* ObjectManager::getObjectByIteration(int index){
	while(index < this->objectContainer.size()){
		Object* object = this->objectContainer[index];
		if(object != NULL)
			return object;
		index++;
	}
	return NULL;
}

void ObjectManager::removeObject(Object* object){
	// Mando a borrar de la lista de objetos al objeto en cuestion
	int position = this->getPositionByObject(object);
	this->removeObject(position);
}

Object* ObjectManager::removeObject(int positionInContainer){
	// Lo saco de la lista
	Object* object = this->popObject(positionInContainer);

	// Borro las luces del objeto del lightingManager
	if(object != NULL){
		vector<ObjectLight*>* objectLights = object->getAttachedLights();
		for(unsigned int i=0; i < objectLights->size(); i++){
			this->lightingManager->removeLight(objectLights->at(i));
		}
	}
	return object;
}

Object* ObjectManager::popObject(int positionInContainer){
	if(positionInContainer >= 0){
		if(this->objectContainer[positionInContainer] != NULL){
			//Lo saco de la lista pero no lo borro de la memoria
			Object* object = this->objectContainer[positionInContainer];
			this->objectContainer[positionInContainer] = NULL;
			//Aumento la cantidad de posiciones libres
			this->posicionesLibres++;
			return object;
		}
	}
	return NULL;
}

void ObjectManager::update(){
	for(int i=0; i < this->objectContainer.size(); i++){
		if(this->objectContainer[i] != NULL)
			this->objectContainer[i]->update();
	}
}

void ObjectManager::render(Camara* camera){
	for(int i=0; i < this->objectContainer.size(); i++){
		if(this->objectContainer[i] != NULL)
			this->objectContainer[i]->draw(camera);
	}
}

void ObjectManager::renderWithSpecificShader(MeshDrawerShader* shaderProgram, Camara* camera){
	for(int i=0; i < this->objectContainer.size(); i++){
		if(this->objectContainer[i] != NULL)
			this->objectContainer[i]->drawWithSpecificShader(shaderProgram, camera);
	}
}

ObjectManager* ObjectManager::clone(LightingManager* lightingManager){
	ObjectManager* clone = new ObjectManager();
	clone->setLightingManager(lightingManager);

	// Clono y agrego cada objeto
	for(unsigned int i=0; i < this->objectContainer.size(); i++){
		Object* cloneObject = this->objectContainer[i]->clone(this->objectContainer[i]->getId());
		clone->addObject(cloneObject);
	}
	return clone;
}

ObjectManager::~ObjectManager(){
	this->deleteAllObjects();
}

void ObjectManager::clear(){
	// Saco de la lista cada objeto y sus luces del lighting manager (no los borro)
	for(unsigned int i=0; i < this->objectContainer.size() ; i++){
		this->removeObject(i);
	}
	this->posicionesLibres = 0;
	this->objectContainer.clear();
}

// Borra fisicamente todos los objetos (no se borran sus luces asociadas)
void ObjectManager::deleteAllObjects(){
	for(int i=0; i < this->objectContainer.size(); i++){
		Object* object = this->removeObject(i);
		if(object != NULL) 
			delete object;
	}
	this->posicionesLibres = 0;
	this->objectContainer.clear();
}