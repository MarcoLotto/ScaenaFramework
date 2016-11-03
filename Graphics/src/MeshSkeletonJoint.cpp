/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "MeshSkeletonJoint.h"
#include "LineManager.h"
#include "GenericUtils.h"
#include "DataConformationException.h"

MeshSkeletonJoint::MeshSkeletonJoint(MeshSkeleton* parentJoint) {	
	this->globalBindposeTransformation = new Transformation();
	this->parentJoint = parentJoint;
	this->lineHandler = -1;
	this->root = false;
}

MeshSkeletonJoint::~MeshSkeletonJoint(){
	delete this->globalBindposeTransformation;

	if(this->lineHandler > 0){
		LineManager::getInstance()->removeLine(this->lineHandler);
		this->lineHandler = -1;
	}
}

void MeshSkeletonJoint::setGlobalBindposeTransformation(Transformation* transformation){
	if(this->globalBindposeTransformation != NULL)
		delete this->globalBindposeTransformation;
	this->globalBindposeTransformation = transformation; 
}

// La llamada para decirle al shader activo que dibuje el mesh
void MeshSkeletonJoint::draw(mat4 modelMatrix, Camara* camera){	
	// Si el padre es el root del skeleton, no dibujo la linea porque no hay union
	if(!this->parentJoint->isRoot()){
		// Verifico si la linea ya fue cargada y esta actualizada y renderizo
		this->checkDrawLineInicialization(modelMatrix);
		LineManager::getInstance()->renderLine(LineShader::getInstance(), camera->getViewMatrix(), camera->getPerspectiveMatrix(), this->lineHandler);
	}
	// Mando a dibujar a todas las joints
	for(unsigned int i = 0; i < this->childJoints->size(); i++){
		this->childJoints->at(i)->draw(modelMatrix, camera);
	}
}

// La llamada para decirle al shader activo que dibuje el mesh
void MeshSkeletonJoint::drawWithSpecificShader(MeshDrawerShader* programShader, mat4 modelMatrix, mat4 viewMatrix, mat4 projMatrix){
	// Si el padre es el root del skeleton, no dibujo la linea porque no hay union
	if(!this->parentJoint->isRoot()){
		// Verifico si la linea ya fue cargada y renderizo
		this->checkDrawLineInicialization(modelMatrix);
		LineManager::getInstance()->renderLine(programShader, viewMatrix, projMatrix, this->lineHandler);
	}

	// Mando a dibujar a todas las joints
	for(unsigned int i = 0; i < this->childJoints->size(); i++){
		this->childJoints->at(i)->drawWithSpecificShader(programShader, modelMatrix, viewMatrix, projMatrix);
	}
}

void MeshSkeletonJoint::update(){	
	//Mando a hacer update a los hijos
	for(int i=0; i < this->childJoints->size(); i++){
		this->childJoints->at(i)->update();
	}
}

void MeshSkeletonJoint::checkDrawLineInicialization(mat4 modelMatrix){
	vec3 startPosition = vec3(modelMatrix*vec4(((MeshSkeletonJoint*) this->parentJoint)->globalBindposeTransformation->getPosition(), 1.0f));
	vec3 endPosition = vec3(modelMatrix*vec4(this->globalBindposeTransformation->getPosition(), 1.0f));

	if(this->lineHandler < 0){
		vec3 lineColor = vec3(256.0f / (float)GenericUtils::getRandomNumber(0, 256), 256.0f / (float)GenericUtils::getRandomNumber(0, 256), 256.0f / (float)GenericUtils::getRandomNumber(0, 256));
		this->lineHandler = LineManager::getInstance()->addLine(startPosition, endPosition, lineColor); 
	}
	LineManager::getInstance()->setPosition(startPosition, endPosition, this->lineHandler);  // Tremendamente no performante	
}

// Interpola el joint actual con el source (usando step entre 0 y 1) y lo guarda en destination
void MeshSkeletonJoint::interpolate(MeshSkeleton* destination, MeshSkeleton* source, float step){
	// Si las estructuras no coinciden, terminamos
	if(this->getChilds()->size() != source->getChilds()->size() || this->getChilds()->size() != destination->getChilds()->size())
		throw new DataConformationException("No se puede interpolar dos skeletons porque su estructura no es la misma (procesnado un joint)");

	// Creo el destination joint e interpolo los joint	
	this->interpolateJointParameters((MeshSkeletonJoint*) destination, (MeshSkeletonJoint*) source, step);
	
	// Mando a procesar a todos los joints hijos
	for(unsigned int i = 0; i < this->childJoints->size(); i++){
		this->childJoints->at(i)->interpolate(destination->getChilds()->at(i), source->getChilds()->at(i), step);
	}
}

vec3 linearInterpolation(vec3 v1, vec3 v2, float step){
	return (1.0f - step) * v1 + step * v2;
}

void MeshSkeletonJoint::interpolateJointParameters(MeshSkeletonJoint* destination, MeshSkeletonJoint* source, float step){
	Transformation* destTransform = destination->getGlobalBindposeTransformation();
	Transformation* sourceTransform = source->getGlobalBindposeTransformation();
	Transformation* myTransform = this->getGlobalBindposeTransformation();

	destTransform->setPosition(linearInterpolation(myTransform->getPosition(), sourceTransform->getPosition(), step)); 
	destTransform->setRotation(linearInterpolation(myTransform->getRotation(), sourceTransform->getRotation(), step)); 
	destTransform->setScale(linearInterpolation(myTransform->getScale(), sourceTransform->getScale(), step)); 
}

MeshSkeletonJoint* MeshSkeletonJoint::clone(MeshSkeleton* parent){
	MeshSkeletonJoint* clone = new MeshSkeletonJoint(parent);
	Transformation* transform = new Transformation();
	this->globalBindposeTransformation->clone(transform);
	clone->setGlobalBindposeTransformation(transform);
	clone->setIndex(this->index);
	clone->setName(this->name);
	
	for(unsigned int i = 0; i < this->childJoints->size(); i++){
		clone->addChild(this->childJoints->at(i)->clone(clone));
	}
	return clone;
}

// Devuelve un joint por su nombre, o NULL si no se encuentra
MeshSkeletonJoint* MeshSkeletonJoint::findJointByName(string name){
	if(this->name.compare(name) == 0)
		return this;

	for(unsigned int i = 0; i < this->childJoints->size(); i++){
		MeshSkeletonJoint* joint = this->childJoints->at(i)->findJointByName(name);
		if(joint != NULL)
			return joint;
	}
	return NULL;
}

// Devuelve un joint por su index id, o NULL si no se encuentra
MeshSkeletonJoint* MeshSkeletonJoint::findJointByIndex(int index){
	if(this->index == index)
		return this;

	for(unsigned int i = 0; i < this->childJoints->size(); i++){
		MeshSkeletonJoint* joint = this->childJoints->at(i)->findJointByIndex(index);
		if(joint != NULL)
			return joint;
	}
	return NULL;
}

// Guarda la estructura del skeleton en un vector ordenado por index de cada joint
void MeshSkeletonJoint::serializeForDrawOrderedByIndex(vector<MeshSkeletonJoint*>* destination, vector<MeshSkeletonJoint*>* pendingJoints, int* nextIndex){
	// Si corresponde al proximo index, lo agrego
	if(this->index == *nextIndex){
		destination->push_back(this);
		(*nextIndex)++;
	}
	else if(this->index > *nextIndex){
		// No es el index indicado, me guardo como pendiente el actual y busco si ya salio este index		
		vector<MeshSkeletonJoint*>::iterator it = pendingJoints->begin();
		while(it != pendingJoints->end()){
			if((*it)->getIndex() == *nextIndex){
				destination->push_back(*it);
				it = pendingJoints->erase(it);
				(*nextIndex)++;
				break;
			}
			else{
				++it;
			}
		}
		pendingJoints->push_back(this);
	}

	for(unsigned int i = 0; i < this->childJoints->size(); i++){
		this->childJoints->at(i)->serializeForDrawOrderedByIndex(destination, pendingJoints, nextIndex);
	}
}

void MeshSkeletonJoint::mixTransformations(Transformation* destinationTransform, MeshSkeletonJoint* otherJoint){
	vec3 positionTransform = otherJoint->getGlobalBindposeTransformation()->getPosition() - this->getGlobalBindposeTransformation()->getPosition();
	vec3 rotationTransform = otherJoint->getGlobalBindposeTransformation()->getRotation() - this->getGlobalBindposeTransformation()->getRotation();
	vec3 scaleTransform = vec3(1.0f) + otherJoint->getGlobalBindposeTransformation()->getScale() - this->getGlobalBindposeTransformation()->getScale();

	destinationTransform->setPosition(positionTransform);
	destinationTransform->setRotation(rotationTransform);
	destinationTransform->setScale(scaleTransform);
}

unsigned int MeshSkeletonJoint::getHash(unsigned int parentHashIndex){
	// Utilizo parametros que diferenciarian un joint de otro
	unsigned int hash = ((parentHashIndex + 1) * this->index * this->childJoints->size()) % 32767;

	// Calculo el hash como la suma del hash de todos los joints
	for(unsigned int i = 0; i < this->childJoints->size(); i++){
		hash = (hash + this->childJoints->at(i)->getHash(i * (parentHashIndex + 1))) % 32767;	
	}
	return hash;
}