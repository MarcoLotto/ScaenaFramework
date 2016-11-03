/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "MeshSkeleton.h"
#include "MeshSkeletonJoint.h"
#include "DataConformationException.h"

MeshSkeleton::~MeshSkeleton(){ 
	for(unsigned int i=0; i < this->childJoints->size(); i++){
		delete this->childJoints->at(i);
	}
	delete this->childJoints;
}

// La llamada para decirle al shader activo que dibuje el mesh
void MeshSkeleton::draw(mat4 modelMatrix, Camara* camera){	
	
	// El root no se dibuja, solo mando a dibujar a todos los joints hijos	

	// Mando a dibujar a todas las joints
	for(unsigned int i = 0; i < this->childJoints->size(); i++){
		this->childJoints->at(i)->draw(modelMatrix, camera);
	}
}

// La llamada para decirle al shader activo que dibuje el mesh
void MeshSkeleton::drawWithSpecificShader(MeshDrawerShader* programShader, mat4 modelMatrix, mat4 viewMatrix, mat4 projMatrix){	
	
	// El root no se dibuja, solo mando a dibujar a todos los joints hijos	

	// Mando a dibujar a todas las joints
	for(unsigned int i = 0; i < this->childJoints->size(); i++){
		this->childJoints->at(i)->drawWithSpecificShader(programShader, modelMatrix, viewMatrix, projMatrix);
	}
}

void MeshSkeleton::update(){	
	//Mando a hacer update a los hijos
	for(int i=0; i < this->childJoints->size(); i++){
		this->childJoints->at(i)->update();
	}
}

// Interpola el skeleton actual con el source (usando step entre 0 y 1) y lo guarda en destination
void MeshSkeleton::interpolate(MeshSkeleton* destination, MeshSkeleton* source, float step){
	// Si las estructuras no coinciden, terminamos
	if(this->getChilds()->size() != source->getChilds()->size() || this->getChilds()->size() != destination->getChilds()->size())
		throw new DataConformationException("No se puede interpolar dos skeletons porque su estructura no es la misma"); 
	
	// Mando a procesar a todos los joints hijos
	for(unsigned int i = 0; i < this->childJoints->size(); i++){
		this->childJoints->at(i)->interpolate(destination->getChilds()->at(i), source->getChilds()->at(i), step);
	}
}

MeshSkeleton* MeshSkeleton::clone(){
	MeshSkeleton* clone = new MeshSkeleton();
	clone->setName(this->name);
	
	for(unsigned int i = 0; i < this->childJoints->size(); i++){
		clone->addChild(this->childJoints->at(i)->clone(clone));
	}
	return clone;
}

// Devuelve un joint por su nombre, o NULL si no se encuentra
MeshSkeletonJoint* MeshSkeleton::findJointByName(string name){
	for(unsigned int i = 0; i < this->childJoints->size(); i++){
		MeshSkeletonJoint* joint = this->childJoints->at(i)->findJointByName(name);
		if(joint != NULL)
			return joint;
	}
	return NULL;
}

// Devuelve un joint por su index id, o NULL si no se encuentra
MeshSkeletonJoint* MeshSkeleton::findJointByIndex(int index){	
	for(unsigned int i = 0; i < this->childJoints->size(); i++){
		MeshSkeletonJoint* joint = this->childJoints->at(i)->findJointByIndex(index);
		if(joint != NULL)
			return joint;
	}
	return NULL;
}

// Guarda la estructura del skeleton en un vector ordenado por index de cada joint
void MeshSkeleton::serializeForDrawOrderedByIndex(vector<MeshSkeletonJoint*>* destination){
	destination->clear();
	vector<MeshSkeletonJoint*> pendingJoints;
	for(unsigned int i = 0; i < this->childJoints->size(); i++){
		int firstIndex = 0;
		this->childJoints->at(i)->serializeForDrawOrderedByIndex(destination, &pendingJoints, &firstIndex);
	}
}

// Devuelve un digest del skeleton, identificando la estructura del mismo y no sus datos. Es decir dos skeletons pueden tener mismo hash con transformaciones de joints distintas. 
unsigned int MeshSkeleton::getHash(){
	if(this->hashNeedsRecalculation){
        this->hash = 0;
		// Calculo el hash como la suma del hash de todos los joints
		for(unsigned int i = 0; i < this->childJoints->size(); i++){
			this->hash = (this->hash + this->childJoints->at(i)->getHash(i)) % 32767;			
		}
		this->hashNeedsRecalculation = false;
	}
	return this->hash;
}