/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SkinnedMesh.h"
#include "MeshSkeletonJoint.h"
#include "GraphicDevice.h"

void SkinnedMesh::initDrawingOptions(){
	this->rootSkinnedMesh = NULL;
	this->animation = NULL;
	this->lastClonnedMesh = NULL;
	this->bindposeSkeletonByIndex = new vector<MeshSkeletonJoint*>();
	this->drawingMesh = true;
	this->drawingSkeleton = false;
}

SkinnedMesh::SkinnedMesh() : Mesh(){
	this->skeleton = NULL;	
	this->initDrawingOptions();
}

SkinnedMesh::SkinnedMesh(MeshSkeleton* skeleton) : Mesh(){	
	this->skeleton = skeleton;		
	this->initDrawingOptions();
	this->skeleton->serializeForDrawOrderedByIndex(this->bindposeSkeletonByIndex);
}

SkinnedMesh::~SkinnedMesh(){
	if(this->animation != NULL){
		delete this->animation;
		this->animation = NULL;
	}
}
	
void SkinnedMesh::init(){
	// No quiero que me inicialize dos veces
	if(this->alreadyInitialized)
		return;
	this->alreadyInitialized = true;

	// Primero mando a cargar en memoria de video los buffers (si ya estan cargados no se hara denuevo)
	this->meshBuffers->load();

	// Mando a calcular el bounding
	this->bounding->recalculate(this->meshBuffers->getVertexBufferObject());
	
	//Mando a cargar los hijos
	std::vector<Mesh*>::iterator it = this->childs->begin();
	while(it != this->childs->end()){
		(*it)->init();
		++it;
	}
}

void SkinnedMesh::update(){
	// Si tengo animación la actualizo
	if(this->animation != NULL){
		this->animation->update();
	}
	//Mando a hacer update a los hijos
	for(int i=0; i < this->childs->size(); i++){
		this->childs->at(i)->update();
	}	
}

// La llamada para decirle al shader activo que dibuje el mesh
void SkinnedMesh::shaderDrawCall(MeshDrawerShader* programShader, mat4 modelMatrix, mat4 viewMatrix, mat4 projMatrix){
	if(this->drawingMesh)
		programShader->draw(this);
	if(this->drawingSkeleton)
		this->drawSkeleton(programShader, modelMatrix, viewMatrix, projMatrix);
}

void SkinnedMesh::drawSkeleton(MeshDrawerShader* programShader, mat4 modelMatrix, mat4 viewMatrix, mat4 projMatrix){
	if(this->animation != NULL){
		this->getAnimation()->getActualPose()->drawWithSpecificShader(programShader, modelMatrix, viewMatrix, projMatrix);
	}
	else if(this->rootSkinnedMesh != NULL && this->rootSkinnedMesh->getAnimation() != NULL){
		this->rootSkinnedMesh->getAnimation()->getActualPose()->drawWithSpecificShader(programShader, modelMatrix, viewMatrix, projMatrix);
	}
	else{
		this->skeleton->drawWithSpecificShader(programShader, modelMatrix, viewMatrix, projMatrix);
	}
}

Mesh* SkinnedMesh::clone(){
	SkinnedMesh* clone = new SkinnedMesh(this->skeleton);
	this->lastClonnedMesh = clone;

	// Encuentro cual es el rootMesh (clonado, no original)
	if(this->rootSkinnedMesh != NULL){
		this->rootSkinnedMesh->getLastClonnedMesh()->asignRootSkinnedMeshToChild(clone);
	}
	// Clono las caracticas nativas del Mesh
	((Mesh*) this)->clone(clone);
	return clone;
}

void SkinnedMesh::deleteSharedResources(){
	delete this->skeleton;
}

void SkinnedMesh::setAnimation(SkinnedAnimation* animation){ 
	if(this->animation != NULL){
		delete this->animation;		
	}
	this->animation = animation; 
} 

void SkinnedMesh::setSmoothAnimation(SkinnedAnimation* animation){ 
	if(this->animation != NULL){
		SkinnedAnimationKeyFrame* actualKeyframe = this->animation->getActualKeyFrame();
		animation->setSmoothKeyFrame(actualKeyframe);				
	}
	this->setAnimation(animation); 
} 

void SkinnedMesh::addChild(SkinnedMesh* child){ 
	this->childs->push_back(child);
	this->asignRootSkinnedMeshToChild(child);
}

void SkinnedMesh::asignRootSkinnedMeshToChild(SkinnedMesh* child){
	// Si tengo root y no tengo animacion, le asigno mi root al child (si el no tiene animacion, sino el es root de por si)
	if(this->rootSkinnedMesh != NULL && this->animation == NULL){
		if(child->getAnimation() == NULL){
			child->setRootSkinnedMesh(this->rootSkinnedMesh);
		}
	}
	else if(child->getAnimation() == NULL){
		child->setRootSkinnedMesh(this);
	}
	else{
		child->setRootSkinnedMesh(NULL);
	}
}

void SkinnedMesh::getJointsFinalTransformations(mat4** destinationFinalPose){
	// Primero encuentro cual es la postura actual
	vector<MeshSkeletonJoint*> actualPose;
	if( this->animation != NULL ){
		this->animation->getActualPose()->serializeForDrawOrderedByIndex(&actualPose);
	}
	else if( this->rootSkinnedMesh != NULL && this->rootSkinnedMesh->getAnimation() != NULL){
		this->rootSkinnedMesh->getAnimation()->getActualPose()->serializeForDrawOrderedByIndex(&actualPose);
	}
	else{
		this->skeleton->serializeForDrawOrderedByIndex(&actualPose);
	}

	// Mergeo los atributos del actualPose con el bindPose	
	this->mixJointsTransformationsAndBuildMatrix(destinationFinalPose, &actualPose);	
}

void SkinnedMesh::mixJointsTransformationsAndBuildMatrix(mat4** finalPose, vector<MeshSkeletonJoint*>* actualPose){
	map<MeshSkeleton*, mat4> parentGlobalTransformations;  
	for(unsigned int i=0; i < this->bindposeSkeletonByIndex->size() && i < actualPose->size(); i++){
		// Consigo las diferencias entre el bind pose y el pose actual
		Transformation finalTransform;
		MeshSkeletonJoint* actualJoint = actualPose->at(i);
		this->bindposeSkeletonByIndex->at(i)->mixTransformations(&finalTransform, actualJoint);

		// Busco una transformacion del padre
		mat4 parentRotMatrix;
		map<MeshSkeleton*, mat4>::iterator it = parentGlobalTransformations.find(actualJoint->getParentJoint());
		if(it != parentGlobalTransformations.end()){
			parentRotMatrix = it->second;
		}

		// Calculamos la matriz de rotacion para este joint
		vec3 bindPoseJointPosition = this->bindposeSkeletonByIndex->at(i)->getGlobalBindposeTransformation()->getPosition();
		mat4 localRotMatrix = glm::translate(mat4(1.0f), bindPoseJointPosition);
		localRotMatrix = glm::rotate(localRotMatrix, finalTransform.getRotation().z, vec3(1.0f, 0.0f, 0.0f));
		localRotMatrix = glm::rotate(localRotMatrix, finalTransform.getRotation().y, vec3(0.0f, 1.0f, 0.0f));		
		localRotMatrix = glm::rotate(localRotMatrix, finalTransform.getRotation().x, vec3(0.0f, 0.0f, 1.0f));
		localRotMatrix = glm::translate(localRotMatrix, -bindPoseJointPosition);

		mat4 globalRotMatrix = parentRotMatrix * localRotMatrix;
		
		// Armo la matriz de transformacion para modificar los vertices y me la guardo						
		(*finalPose)[i] = globalRotMatrix;

		// Guardo la rotacion de este joint paa que un hijo lo pueda usar
		parentGlobalTransformations[actualJoint] = globalRotMatrix;
	}
}