/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Camara.h"
#include "MeshDrawerShader.h"
#include "glm/glm.hpp"
#include <string>
#include <vector>
using namespace std;
typedef glm::mat4 mat4;

class MeshSkeletonJoint;

// Representa un esqueleto de un mesh. Este objeto es siempre el root y esta compuesto por joints.
class MeshSkeleton {

protected:
	string name;	
	bool root;

	unsigned int hash;
	bool hashNeedsRecalculation;

	vector<MeshSkeletonJoint*>* childJoints;	

public:
	MeshSkeleton(){ this->childJoints = new vector<MeshSkeletonJoint*>(); this->root = true; this->hashNeedsRecalculation = true; }
	virtual ~MeshSkeleton();

	string getName(){ return this->name; }			// Nombre con el que se identifica
	
	void setName(string name){ this->name = name; }
	
	void addChild(MeshSkeletonJoint* joint){ this->childJoints->push_back(joint); this->hashNeedsRecalculation = true; }
	vector<MeshSkeletonJoint*>* getChilds(){ return this->childJoints; }

	// Interpola el skeleton actual con el source (usando step entre 0 y 1) y lo guarda en destination
	virtual void interpolate(MeshSkeleton* destination, MeshSkeleton* source, float step);

	// Identifica si es el root del skeleton o un joint
	bool isRoot(){ return this->root; }

	// Clona el esqueleto actual
	MeshSkeleton* clone();

	// Devuelve un joint por su nombre o index, o NULL si no se encuentra
	MeshSkeletonJoint* findJointByName(string name);
	MeshSkeletonJoint* findJointByIndex(int index);

	// Guarda la estructura del skeleton en un vector ordenado por index de cada joint
	void serializeForDrawOrderedByIndex(vector<MeshSkeletonJoint*>* destination);

	// Devuelve un digest del skeleton, identificando la estructura del mismo y no sus datos. Es decir dos skeletons pueden tener mismo hash con transformaciones de joints distintas. 
	unsigned int getHash();

	// ***************** Primitivas para el dibujado del esqueleto***********************	
	virtual void draw(mat4 modelMatrix, Camara* camera);
	virtual void drawWithSpecificShader(MeshDrawerShader* programShader, mat4 modelMatrix, mat4 viewMatrix, mat4 projMatrix);
	virtual void update();
	// ***********************************************************************************
};