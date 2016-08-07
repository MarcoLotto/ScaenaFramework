/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ObjectFactory.h"
#include "MeshFactory.h"
#include "ObjectLoader.h"

void ObjectFactory::fillObjectData(Object* finalObject, Mesh* mesh, vec3 position, vec3 rotation, vec3 scale){		
	finalObject->setMesh(mesh);
	Transformation* transformation = finalObject->getTransformation();
	transformation->setPosition(position);
	transformation->setRotation(rotation);
	transformation->setScale(scale);	
}

Object* ObjectFactory::createObjectFromMesh(string id, Mesh* mesh, vec3 position, vec3 rotation, vec3 scale){
	Object* object = new Object(id);
	fillObjectData(object, mesh, position, rotation, scale);
	return object;
}

Object* ObjectFactory::createObjectFromMeshFile(string id, const char* meshFilename, vec3 position, vec3 rotation, vec3 scale){
	Mesh* mesh = MeshFactory::getInstance()->createMeshFromFile(meshFilename);
	return createObjectFromMesh(id, mesh, position, rotation, scale);
}

Object* ObjectFactory::loadObjectFromFile(const char* filename){
	return ObjectLoader::loadObjectFromFile(filename);
}

void ObjectFactory::loadObjectFromFile(Object* finalObject, const char* filename){
	Object* basicObject = loadObjectFromFile(filename);
	Transformation* transform = basicObject->getTransformation();
	fillObjectData(finalObject, basicObject->getMesh(), transform->getPosition(), transform->getRotation(), transform->getScale());
}