/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "Mesh.h"
#include "GraphicDevice.h"

Mesh::Mesh(){	
	this->name = "";
	this->shaderProgram = NULL;	
	this->filePath = "";
	this->childs = new vector<Mesh*>();
	this->alreadyInitialized = false;
	this->isDrawDataDirty = true;

	//Inicializo los recursos
	this->initiateResources();
}

// Inicializa los recursos del mesh
void Mesh::initiateResources(){
	this->transformation = new Transformation();
	this->meshMaterial = new MeshMaterial();
	this->meshBuffers = new MeshBuffers();
	this->diffuseMap = new DiffuseMap();
	this->normalMap = new NormalMap();
	this->displacementMap = new DisplacementMap();
	this->uvTransformations = new UVTransformations();
	this->bounding = new BoundingBox(); 
	this->drawPacket = new DrawPacket();
}

// Borra los recursos del mesh
void Mesh::deleteResources(){
	delete this->transformation;
	delete this->meshMaterial;
	delete this->meshBuffers;
	delete this->diffuseMap;
	delete this->normalMap;
	delete this->displacementMap;
	delete this->uvTransformations;
	delete this->bounding;
	delete this->drawPacket;
}


void Mesh::setShaderProgram(MeshDrawerShader* shader){
	this->shaderProgram = shader;
}

MeshDrawerShader* Mesh::getShaderProgram(){
	return this->shaderProgram;
}

Mesh::~Mesh(){
	// Borro todos los recursos
	this->deleteResources();

	//Borro los hijos y el vector de hijos
	std::vector<Mesh*>::iterator it = this->childs->begin();
	while(it != this->childs->end()){
		delete (*it);
		it = this->childs->erase(it);
	}
	delete this->childs;
}

void Mesh::init(){
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

void Mesh::internalPreloadDrawData(mat4 parentModelMatrix, Camara* camera, bool needRecalculation, bool childsRecalculation){
	// Primero calculo la matriz de modelo global
	mat4 modelMatrix = this->transformation->getGlobalModelMatrix(parentModelMatrix, needRecalculation, &childsRecalculation);
	
	// Actualizo el bounding del mesh
	if(childsRecalculation){
		this->bounding->updateTransformationMatrix(modelMatrix);
	}
	// Calculo el paquete de dibujado para este mesh
	this->drawPacket->setModelMatrix(modelMatrix);
	this->drawPacket->setViewMatrix(camera->getViewMatrix());
	this->drawPacket->setProjMatrix(camera->getPerspectiveMatrix());
	this->drawPacket->calculateMatrixDrawData();

	// Indico que este frame ya fue actualizada la draw data, por lo que no esta dirty
	this->isDrawDataDirty = false;

	// Calculo el draw data de sus mesh hijos
	for(int i=0; i < this->childs->size(); i++){
		this->childs->at(i)->internalPreloadDrawData(modelMatrix, camera, needRecalculation, childsRecalculation);
	}
}

void Mesh::preloadDrawData(mat4 modelMatrix, Camara* camera){
	bool needsRecalculation = this->transformation->hasParentModelMatrixChanged(modelMatrix);
	this->internalPreloadDrawData(modelMatrix, camera, needsRecalculation, false);
}

void Mesh::draw(mat4 modelMatrix, Camara* camera){	
	this->draw(modelMatrix, camera, false);
}

void Mesh::draw(mat4 parentGlobalModelMatrix, Camara* camera, bool needRecalculation){
	// Si no se hizo precalculo de informacion de dibujado, lo hago ahora
	if(this->isDrawDataDirty){
		this->preloadDrawData(parentGlobalModelMatrix, camera);
	}
	bool childsRecalculation;
	mat4 modelMatrix = this->transformation->getGlobalModelMatrix(parentGlobalModelMatrix, false, &childsRecalculation);
	
	// Pregunto si el mesh es visible y dibujo
	if((this->meshBuffers->isAlreadyLoaded())&&(this->shaderProgram != NULL)){
		if(camera->getViewFrustum()->isVisible(this->bounding)){ 		
			//Configuro el shader a utilizar
			vec3 centerPoint = vec3(modelMatrix * vec4(0.0f, 0.0f, 0.0f, 1.0f));
			this->shaderProgram = this->shaderProgram->evaluateAndChangeShader(&this->shaderHistory, centerPoint);
			//Mando a que el shader dibuje
			this->shaderDrawCall(this->shaderProgram, modelMatrix, camera->getViewMatrix(), camera->getPerspectiveMatrix());			
		}
	}
	// Ya dibuje, tengo que recalcular draw data
	this->isDrawDataDirty = true; 

	//Mando a dibujar los hijos
	for(int i=0; i < this->childs->size(); i++){
		this->childs->at(i)->draw(modelMatrix, camera, false);
	}
}

void Mesh::drawWithSpecificShader(MeshDrawerShader* programShader, mat4 modelMatrix, Camara* camera){	
	this->drawWithSpecificShader(programShader, modelMatrix, camera, false);
}

void Mesh::drawWithSpecificShader(MeshDrawerShader* programShader, mat4 parentGlobalModelMatrix, Camara* camera, bool needRecalculation){
	// Si no se hizo precalculo de informacion de dibujado, lo hago ahora
	if(this->isDrawDataDirty){
		this->preloadDrawData(parentGlobalModelMatrix, camera);
	}
	bool childsRecalculation;
	mat4 modelMatrix = this->transformation->getGlobalModelMatrix(parentGlobalModelMatrix, false, &childsRecalculation);

	// Pregunto si el mesh es visible y dibujo
	if(this->meshBuffers->isAlreadyLoaded()){
		if(camera->getViewFrustum()->isVisible(this->bounding)){ 	
			//Mando a que el shader dibuje
			this->shaderDrawCall(programShader, modelMatrix, camera->getViewMatrix(), camera->getPerspectiveMatrix());	
		}
	}
	// Ya dibuje, tengo que recalcular draw data
	this->isDrawDataDirty = true; 

	//Mando a dibujar los hijos
	for(int i=0; i < this->childs->size(); i++){
		this->childs->at(i)->drawWithSpecificShader(programShader, modelMatrix, camera, false);
	}
}

void  Mesh::update(){
	//Mando a hacer update a los hijos
	for(int i=0; i < this->childs->size(); i++){
		this->childs->at(i)->update();
	}
}

// La llamada para decirle al shader activo que dibuje el mesh
void Mesh::shaderDrawCall(MeshDrawerShader* programShader, mat4 modelMatrix, mat4 viewMatrix, mat4 projMatrix){	
	programShader->draw(this);
}

Mesh* Mesh::clone(){
	Mesh* clone = new Mesh();
	this->clone(clone);
	return clone;
}

void Mesh::clone(Mesh* clone){
	clone->setName(this->name);
	clone->setFilePath(this->filePath);
		
	// Clono cada recurso
	this->transformation->clone(clone->getTransformation());
	this->meshMaterial->clone(clone);
	this->meshBuffers->clone(clone->getMeshBuffers());
	this->diffuseMap->clone(clone);
	this->normalMap->clone(clone);
	this->displacementMap->clone(clone);
	this->uvTransformations->clone(clone);
	this->bounding->clone(clone->getBounding());

	// Guardo el shader original de este mesh
	if(this->shaderHistory.size() > 0)
		clone->setShaderProgram(this->shaderHistory[0]);
	else
		clone->setShaderProgram(this->shaderProgram);

	//Proceso sus hijos
	for(int i=0; i < this->childs->size(); i++){
		clone->addChild(this->childs->at(i)->clone());
	}
}

Mesh* Mesh::findSubMesh(string name){
	// Busco primero en los hijos directos
	std::vector<Mesh*>::iterator it = this->childs->begin();
	while(it != this->childs->end()){
		Mesh* mesh = (*it);
		if(mesh->getName().compare(name) == 0){
			return mesh;
		}
		++it;
	}
	//No es un hijo directo, mando a buscar en los hijos de los hijos
	it = this->childs->begin();
	while(it != this->childs->end()){
		Mesh* mesh = (*it)->findSubMesh(name);
		if(mesh != NULL){
			return mesh;
		}
		++it;
	}
	return NULL;
}

bool Mesh::deleteSubMesh(string name){
	// Busco primero en los hijos directos, si lo encuentro lo borro
	std::vector<Mesh*>::iterator it = this->childs->begin();
	while(it != this->childs->end()){
		Mesh* mesh = (*it);
		if(mesh->getName().compare(name) == 0){
			it = this->childs->erase(it);
			delete mesh;
			return true;
		}
		++it;		
	}
	//No es un hijo directo, mando a borrar en los hijos de los hijos
	it = this->childs->begin();
	while(it != this->childs->end()){
		if((*it)->deleteSubMesh(name)){
			return true;
		}
		++it;
	}
	return false;
}

bool Mesh::swapSubMesh(string nameToRemove, Mesh* meshToInsert){
	// Busco primero en los hijos directos, si lo encuentro lo borro e inserto el nuevo mesh
	std::vector<Mesh*>::iterator it = this->childs->begin();
	while(it != this->childs->end()){
		Mesh* mesh = (*it);
		if(mesh->getName().compare(nameToRemove) == 0){
			this->childs->erase(it);
			this->childs->push_back(meshToInsert);
			delete mesh;
			return true;
		}
		++it;
	}
	//No es un hijo directo, mando a hacer swap en los hijos de los hijos
	it = this->childs->begin();
	while(it != this->childs->end()){
		if((*it)->swapSubMesh(nameToRemove, meshToInsert)){
			return true;
		}	
		++it;
	}
	return false;
}

bool Mesh::isColliding(Mesh* otherMesh){
	//**********************************************************************************************
	// REVIEW: Esta no es la mejor forma (mas optima) de hacer esto!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//*********************************************************************************************

	// Para cada submesh, verifico si colisiona con cada submesh del otro mesh
	for(unsigned int i=0; i < otherMesh->getChilds()->size(); i++){
		//Para cada hijo de este mesh, verifico si colisiona
		for(unsigned int a=0; a < this->getChilds()->size(); a++){
			if(this->getChilds()->at(a)->isColliding(otherMesh)){
				return true;
			}
		}
		// Verifico si el mesh actual colisiona
		if(this->getMeshBuffers()->isAlreadyLoaded()){
			if(this->bounding->isColliding(otherMesh->getChilds()->at(i)->getBounding())){
				return true;
			}
		}
	}
	return false;
}

bool Mesh::isIntersecting(LineR3* line){
	list<vec3> lineIntersectionPoints;
	return this->isIntersecting(line, &lineIntersectionPoints);
}

bool Mesh::isIntersecting(LineR3* line, list<vec3>* lineIntersectionPoints){
	//**********************************************************************************************
	// REVIEW: Esta no es la mejor forma (mas optima) de hacer esto!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//*********************************************************************************************
	
	//Para cada hijo de este mesh, verifico si colisiona
	for(unsigned int a=0; a < this->getChilds()->size(); a++){
		if(this->getChilds()->at(a)->isIntersecting(line, lineIntersectionPoints)){
			return true;
		}
	}
	// Verifico si el mesh actual colisiona
	if(this->getMeshBuffers()->isAlreadyLoaded()){
		if(this->bounding->isIntersecting(line)){
			vec3 absBB1CenterPoint = (this->bounding->getAbsoluteMaxPoint() + this->bounding->getAbsoluteMinPoint()) / 2.0f;
			lineIntersectionPoints->push_back(line->getOrthogonalProyectedPoint(absBB1CenterPoint));
			return true;
		}
	}	
	return false;
}