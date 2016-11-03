/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

#include "Object.h"

typedef glm::mat4 mat4;

class ObjectFactory{

public:
	//Construye un object basico a partir de un mesh pre-cargado e informacion de transformacion
	static Object* createObjectFromMesh(string id, Mesh* mesh, vec3 position, vec3 rotation, vec3 scale);

	//Construye un object basico, cargando el mesh via un filename y con la informacion de transformacion
	static Object* createObjectFromMeshFile(string id, const char* meshFilename, vec3 position, vec3 rotation, vec3 scale);

	//Carga un objecto de un archivo xml de objeto
	static Object* loadObjectFromFile(const char* filename);

	// Carga una instancia de object con la informacion de un archivo xml de objeto (sirve para cargar instancias mas complejas de object, como herencias)
	static void loadObjectFromFile(Object* finalObject, const char* filename);

	// Carga una instancia de object con la informacion basica de mesh y transformacion
	static void fillObjectData(Object* finalObject, Mesh* mesh, vec3 position, vec3 rotation, vec3 scale);

};