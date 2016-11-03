/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "Object.h"
#include "ObjectTransformation.h"

Object::Object(string id){
	this->id = id;
	this->filename = "";
	this->mesh = NULL;
	this->attachedLights = new vector<ObjectLight*>();
	this->transformation = new ObjectTransformation(this->attachedLights);
}

Object::~Object(){
	if(this->mesh != NULL){
		delete mesh;
	}
	delete this->transformation;

	// Borro la lista de luces del objeto pero no las luces en si porque las borra el LightingManager
	// REVIEW: No se estan borrando las luces de aquellos objetos que no estan en una escena
	delete this->attachedLights;
}

void Object::update(){
	this->mesh->update();
}

void Object::setMesh(Mesh* mesh){
	this->mesh = mesh;
}
Mesh* Object::getMesh(){
	return this->mesh;
}

void Object::setId(string id){
	this->id = id;
}
string Object::getId(){
	return this->id;
}

void Object::draw(mat4 modelMatrix, Camara* camera){
	bool childsRecalc;
	this->mesh->draw(this->transformation->getGlobalModelMatrix(modelMatrix, true, &childsRecalc), camera);
}

void Object::draw(Camara* camera){
	this->mesh->draw(this->transformation->getLocalModelMatrix(), camera);
}

void Object::drawWithSpecificShader(MeshDrawerShader* programShader, Camara* camera){
	this->mesh->drawWithSpecificShader(programShader, this->transformation->getLocalModelMatrix(), camera);
}

Object* Object::clone(string id){
	Object* clone = this->basicClone(id);
	clone->setFilename(this->filename);
	this->transformation->clone(clone->getTransformation());
	
	if(this->mesh != NULL){
		clone->setMesh(this->mesh->clone());
	}
	// Clono las luces
	for(unsigned int i=0; i < this->getAttachedLights()->size(); i++){
		ObjectLight* light = this->getAttachedLights()->at(i);
		ObjectLight* clonedLight = (ObjectLight*) light->clone();
		clone->addLight(clonedLight);
	}
	return clone;
}

Object* Object::basicClone(string id){
	 return new Object(id);
}