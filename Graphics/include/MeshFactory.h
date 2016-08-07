/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

#include "ScaenaModelLoader.h"
#include "Mesh.h"
#include <map>
using namespace std;

//Crea meshes. Guarda instancias originales de los que va creando y devuelve copias de los mismos.
//De esta forma se ahorra memoria de la carga de multiples meshes iguales.
class MeshFactory{

private:
	map<string, Mesh*> meshInstances;

	ScaenaModelLoader scaenaModelLoader;
	static MeshFactory* instance;

public:
	static MeshFactory* getInstance();

	//Crea un mesh a partir de la ruta de un archivo FBX dado
	Mesh* createMeshFromFile(string filename);

	//Devuelve un mesh de un rectangulo
	Mesh* createRectangleMesh();

	//Devuelve un mesh de un rectangulo centrado en la esquina superior izquierda
	Mesh* createUIRectangleMesh();

	//Borra un mesh por su filename
	void destroyMesh(string filename);

	//Manda a borrar todos los meshes de referencia
	void destroy();

};