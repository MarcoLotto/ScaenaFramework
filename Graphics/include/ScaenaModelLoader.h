/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "XmlParser.h"
#include "MeshDrawerShader.h"
#include "MeshSkeleton.h"
#include "Transformation.h"

class Mesh;

class ScaenaModelLoader{

private:		
	void processMeshTextures(Mesh* mesh, XmlTreeNode* meshNode);
	void processMesh(Mesh* parentMesh, XmlTreeNode* meshNode, MeshSkeleton* skeleton);	
	
	Mesh* getRequiredMeshInstance(MeshSkeleton* skeleton);
	MeshDrawerShader* asignarShader(Mesh* mesh, bool hasSkeleton);

public:
	ScaenaModelLoader();
	virtual ~ScaenaModelLoader();
	
	// Carga un archivo fbx de disco, devuelve true si todo salio bien, false de lo contrario
	Mesh* loadModel(const char* filename);

};