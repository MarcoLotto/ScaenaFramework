/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "glm/glm.hpp"
#include "ControlPoint.h"
#include "InformationDataTransfer.h"
#include <string>
#include <list>
using namespace std;

typedef glm::vec3 vec3;
typedef glm::vec2 vec2;

class MeshDataTransfer : public InformationDataTransfer{

private:
	list<MeshDataTransfer*>* childMesh;	

public:
	MeshDataTransfer(){ this->childMesh = new list<MeshDataTransfer*>(); this->controlPoints = new vector<ControlPoint*>(); this->hasSkiningInfo=false; }
	virtual ~MeshDataTransfer(){ delete this->childMesh; delete this->controlPoints; }

	float* vertexBufferData;
	float* normalBufferData;
	float* tangentBufferData;
	float* binormalBufferData;
	float* textureCoordsData;
	unsigned int* indexBufferData;

	int vertexBufferSize;
	int normalBufferSize;
	int tangentBufferSize;
	int binormalBufferSize;
	int textureCoordsSize;
	int indexBufferSize;

	string name;

	string difuseMapFilename;
	string normalMapFilename;
	string displacementMapFilename;
	float displacementFactor;

	vec2 uvTransform;
	vec2 uvScale;	
	
	// Devuelve informacion sobre los puntos de control, como por ejemplo el vinculo de cada uno sobre los joints de un skelleton. 
	// Un controlPoint = 1 vertice (vec3). Estan en el mismo orden que vertexBufferData (pero dividido 3)
	vector<ControlPoint*>* controlPoints;  
	bool hasSkiningInfo;

	void addChild(MeshDataTransfer* mdt){ this->childMesh->push_back(mdt); } 
	list<MeshDataTransfer*>* getChilds(){ return this->childMesh; }
};