/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <glm/glm.hpp>
#include "MeshSkeleton.h"
#include "Transformation.h"
typedef glm::mat4 mat4;

// Representa un Joint de un skeleton. Es decir una articulación o punto de control del esqueleto.
class MeshSkeletonJoint : public MeshSkeleton{

private:
	int lineHandler;
	void checkDrawLineInicialization(mat4 modelMatrix);
	void interpolateJointParameters(MeshSkeletonJoint* destination, MeshSkeletonJoint* source, float step);

protected:	
	Transformation* globalBindposeTransformation;  // Las transformaciones que tenia el joint cuando fue bindeado con el mesh
	MeshSkeleton* parentJoint;				   // Define quien es el joint padre, osea el que tiene accion directa sobre el
	unsigned int index;
	
public:
	MeshSkeletonJoint(MeshSkeleton* parentJoint);
	virtual ~MeshSkeletonJoint();

	unsigned int getIndex(){ return this->index; }  // Indice con el que se relaciona con los vertices
	void setIndex(unsigned int index){ this->index = index; }

	void setGlobalBindposeTransformation(Transformation* transformation);
	Transformation* getGlobalBindposeTransformation(){ return this->globalBindposeTransformation; }	

	void setParentJoint(MeshSkeleton* parentJoint){ this->parentJoint = parentJoint; }
	MeshSkeleton* getParentJoint(){ return this->parentJoint; }

	// Interpola el joint actual con el source (usando step entre 0 y 1) y lo guarda en destination
	virtual void interpolate(MeshSkeleton* destination, MeshSkeleton* source, float step);

	MeshSkeletonJoint* clone(MeshSkeleton* parent);

	// Devuelve un joint por su nombre o index, o NULL si no se encuentra
	MeshSkeletonJoint* findJointByName(string name);
	MeshSkeletonJoint* findJointByIndex(int index);

	// Guarda la estructura del skeleton en un vector ordenado por index de cada joint
	void serializeForDrawOrderedByIndex(vector<MeshSkeletonJoint*>* destination, vector<MeshSkeletonJoint*>* pendingJoints, int* nextIndex);

	// Mezcla las transformaciones de este joint con otro joint
	void mixTransformations(Transformation* destinationTransform, MeshSkeletonJoint* otherJoint);

	// Devuelve el hash de este joint y el de sus hijos. Recive un parámetro i para agregar mas dispersión al hash.
	unsigned int getHash(unsigned int i);

	// ***************** Primitivas para el dibujado del esqueleto***********************	
	virtual void draw(mat4 modelMatrix, Camara* camera);
	virtual void drawWithSpecificShader(MeshDrawerShader* programShader, mat4 modelMatrix, mat4 viewMatrix, mat4 projMatrix);
	virtual void update();
	// ***********************************************************************************
};