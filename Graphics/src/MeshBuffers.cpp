/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "MeshBuffers.h"
#include "GraphicDevice.h"
#include "Mesh.h"
#include <stdlib.h>

MeshBuffers::MeshBuffers(){
	this->alreadyLoaded = false;
	this->vaoObject = NULL;
	
	this->vertexBufferObject = NULL;
	this->textureCoordsBufferObject = NULL;
	this->normalBufferObject = NULL;
	this->tangentBufferObject = NULL;
	this->binormalBufferObject = NULL;
	this->indexBufferObject = NULL;
	this->jointIndexBufferObject = NULL;
	this->jointWeightBufferObject = NULL;

	this->vertexBuffer = NULL;
	this->textureCoords = NULL;
	this->normalBuffer = NULL;
	this->binormalBuffer = NULL;
	this->tangentBuffer = NULL;
	this->indexBuffer = NULL;
	this->jointIndexBuffer = NULL;
	this->jointWeightBuffer = NULL;

	this->vertexBufferSize = 0;
	this->normalBufferSize = 0;
	this->tangentBufferSize = 0;
	this->binormalBufferSize = 0;
	this->textureBufferSize = 0;
	this->indexBufferSize = 0;
	this->jointIndexBufferSize = 0;
	this->jointWeightBufferSize = 0;
}

MeshBuffers::~MeshBuffers(){
    // REVIEW: No se esta borrando la informacion de buffers, pero no se puede borrar asi nomas. Cada buffer tiene un handler en memoria de video que comparte con
    //         sus clones para ahorrar memoria. Al borrar un buffer, se pide que se borre el handler en memoria de video, por lo que no se que pasa con los clones.
    /*
    if(this->alreadyLoaded){
        if(this->vertexBufferObject != NULL) delete this->vertexBufferObject;
        if(this->textureCoordsBufferObject != NULL) delete this->textureCoordsBufferObject;
        if(this->normalBufferObject != NULL) delete this->normalBufferObject;
        if(this->tangentBufferObject != NULL) delete this->tangentBufferObject;
        if(this->binormalBufferObject != NULL) delete this->binormalBufferObject;
        if(this->indexBufferObject != NULL) delete this->indexBufferObject;
        if(this->jointIndexBufferObject != NULL) delete this->jointIndexBufferObject;
        if(this->jointWeightBufferObject != NULL) delete this->jointWeightBufferObject;
      
        if(this->vertexBuffer != NULL) delete this->vertexBuffer;
        if(this->textureCoords != NULL) delete this->textureCoords;
        if(this->normalBuffer != NULL) delete this->normalBuffer;
        if(this->binormalBuffer != NULL) delete this->binormalBuffer;
        if(this->tangentBuffer != NULL) delete this->tangentBuffer;
        if(this->indexBuffer != NULL) delete this->indexBuffer;
        if(this->jointIndexBuffer != NULL) delete this->jointIndexBuffer;
        if(this->jointWeightBuffer != NULL) delete this->jointWeightBuffer;
    }
    */
}

void MeshBuffers::load(){
	// Evito que cargue si ni siquiera hay un vertex buffer
	if(this->vertexBuffer==NULL){
		return;
	}	

	// Solo permito que se cargue una vez en memoria de video
	if(!this->alreadyLoaded){
		// Creo el vertex array buffer para contener los VBO
		this->vaoObject = new VertexArrayObject();		

		// Dependiendo de que buffers esten seteados, hago la carga
		if(this->vertexBuffer != NULL){
			this->vertexBufferObject = new VertexBufferObject(this->vertexBuffer, this->vertexBufferSize, 3);
			this->vertexBufferObject->load(vaoObject);			
		}
		if(this->normalBuffer != NULL){
			this->normalBufferObject = new VertexBufferObject(this->normalBuffer, this->normalBufferSize, 3);
			this->normalBufferObject->load(vaoObject);		
		}
		if(this->textureCoords != NULL){
			this->textureCoordsBufferObject = new VertexBufferObject(this->textureCoords, this->textureBufferSize, 2);	
			this->textureCoordsBufferObject->load(vaoObject);			
		}
		if(this->tangentBuffer != NULL){
			this->tangentBufferObject = new VertexBufferObject(this->tangentBuffer, this->tangentBufferSize, 3);
			this->tangentBufferObject->load(vaoObject);			
		}
		if(this->binormalBuffer != NULL){
			this->binormalBufferObject = new VertexBufferObject(this->binormalBuffer, this->binormalBufferSize, 3);
			this->binormalBufferObject->load(vaoObject);
		}
		if(this->indexBuffer != NULL){
			this->indexBufferObject = new IndexBufferObject(this->indexBuffer, this->indexBufferSize);
			this->indexBufferObject->load(vaoObject);			
		}
		if(this->jointIndexBuffer != NULL){
			this->jointIndexBufferObject = new VertexBufferObject(this->jointIndexBuffer, this->jointIndexBufferSize, 4);
			this->jointIndexBufferObject->load(vaoObject);			
		}	
		if(this->jointWeightBuffer != NULL){
			this->jointWeightBufferObject = new VertexBufferObject(this->jointWeightBuffer, this->jointWeightBufferSize, 4);
			this->jointWeightBufferObject->load(vaoObject);			
		}
		this->alreadyLoaded = true;
	}
}

void MeshBuffers::clone(MeshBuffers* clone){	
	clone->setVertexBuffer(this->vertexBuffer, this->vertexBufferSize);
	clone->setTextureCoords(this->textureCoords, this->textureBufferSize);
	clone->setNormalBuffer(this->normalBuffer, this->normalBufferSize);
	clone->setTangentBuffer(this->tangentBuffer, this->tangentBufferSize);
	clone->setBinormalBuffer(this->binormalBuffer, this->binormalBufferSize);
	clone->setIndexBuffer(this->indexBuffer, this->indexBufferSize);
	clone->setJointIndexBuffer(this->jointIndexBuffer, this->jointIndexBufferSize);
	clone->setJointWeightBuffer(this->jointWeightBuffer, this->jointWeightBufferSize);

	if(this->getVertexBufferObject() != NULL)
		clone->setVertexBufferObject(this->getVertexBufferObject()->clone());
	if(this->getNormalBufferObject() != NULL)
		clone->setNormalBufferObject(this->getNormalBufferObject()->clone());
	if(this->getTangentBufferObject() != NULL)
		clone->setTangentBufferObject(this->getTangentBufferObject()->clone());
	if(this->getBinormalBufferObject() != NULL)
		clone->setBinormalBufferObject(this->getBinormalBufferObject()->clone());
	if(this->getTextureCoordsBufferObject() != NULL)
		clone->setTextureCoordsBufferObject(this->getTextureCoordsBufferObject()->clone());
	if(this->getIndexBufferObject() != NULL)
		clone->setIndexBufferObject(this->getIndexBufferObject()->clone());
	if(this->getJointIndexBufferObject() != NULL)
		clone->setJointIndexBufferObject(this->getJointIndexBufferObject()->clone());
	if(this->getJointWeightBufferObject() != NULL)
		clone->setJointWeightBufferObject(this->getJointWeightBufferObject()->clone());

	if(this->getVaoObject() != NULL)
		clone->setVaoObject(this->getVaoObject()->clone());
	clone->setAlreadyLoaded(this->alreadyLoaded);
}

void MeshBuffers::cloneUninitialized(MeshBuffers* clone){	
	clone->setVertexBuffer(this->vertexBuffer, this->vertexBufferSize);
	clone->setTextureCoords(this->textureCoords, this->textureBufferSize);
	clone->setNormalBuffer(this->normalBuffer, this->normalBufferSize);
	clone->setTangentBuffer(this->tangentBuffer, this->tangentBufferSize);
	clone->setBinormalBuffer(this->binormalBuffer, this->binormalBufferSize);
	clone->setIndexBuffer(this->indexBuffer, this->indexBufferSize);
	clone->setJointIndexBuffer(this->jointIndexBuffer, this->jointIndexBufferSize);
	clone->setJointWeightBuffer(this->jointWeightBuffer, this->jointWeightBufferSize);
	clone->setAlreadyLoaded(false);
}