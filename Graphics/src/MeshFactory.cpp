/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/
 
#include "MeshFactory.h"
#include "RectangleMeshFactory.h"

MeshFactory* MeshFactory::instance = NULL;

MeshFactory* MeshFactory::getInstance(){
	if(instance == NULL)
		instance = new MeshFactory();
	return instance;
}

Mesh* MeshFactory::createMeshFromFile(string filename){
	map<string, Mesh*>::iterator it = this->meshInstances.find(filename);	
	if(it != this->meshInstances.end())
	{
		//Mesh encontrado, devuelvo una copia sin cargarla de disco
		return it->second->clone();		
	}
	//Elemento no encontrado, lo cargo y lo guardo en el mapa
	Mesh* meshOriginal = this->scaenaModelLoader.loadModel(filename.c_str());	
	if(meshOriginal == NULL){
		// Error al cargar el mesh		
		delete meshOriginal;
		return NULL;
	}
	meshOriginal->init();
	this->meshInstances.insert(std::pair<string, Mesh*>(filename, meshOriginal));
	return meshOriginal->clone();
}

Mesh* MeshFactory::createRectangleMesh(){
	map<string, Mesh*>::iterator it = this->meshInstances.find("RectangleMesh");	
	if(it != this->meshInstances.end())
	{
		//Mesh encontrado, devuelvo una copia sin cargarla de disco
		return it->second->clone();		
	}
	//Elemento no encontrado, lo cargo y lo guardo en el mapa
	Mesh* meshOriginal = RectangleMeshFactory::createCenteredRectangleMesh();
	this->meshInstances.insert(std::pair<string, Mesh*>("RectangleMesh", meshOriginal));
	return meshOriginal->clone();
}

Mesh* MeshFactory::createUIRectangleMesh(){
	map<string, Mesh*>::iterator it = this->meshInstances.find("UIRectangleMesh");	
	if(it != this->meshInstances.end())
	{
		//Mesh encontrado, devuelvo una copia sin cargarla de disco
		return it->second->clone();		
	}
	//Elemento no encontrado, lo cargo y lo guardo en el mapa
	Mesh* meshOriginal = RectangleMeshFactory::createUIRectangleMesh();
	this->meshInstances.insert(std::pair<string, Mesh*>("UIRectangleMesh", meshOriginal));
	return meshOriginal->clone();
}

void MeshFactory::destroy(){
	map<string, Mesh*>::iterator it = this->meshInstances.begin();
	while(it != this->meshInstances.end()){
		Mesh* mesh = it->second;
		if(mesh != NULL){
			mesh->deleteSharedResources();
			delete mesh;
		}
		it->second = NULL;
		it++;
	}
	this->meshInstances.clear();
}

void MeshFactory::destroyMesh(string filename){
	Mesh* mesh;
	map<string, Mesh*>::iterator it = this->meshInstances.find(filename);	
	if(it != this->meshInstances.end())
	{	
		//El elemento esta en el mapa
		mesh = it->second;
		if(mesh != NULL){
			mesh->deleteSharedResources();
			delete mesh;
		}
		//Lo borro del mapa
		this->meshInstances.erase(it);		
	}	
}