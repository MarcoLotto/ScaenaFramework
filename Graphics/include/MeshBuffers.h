/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"
#include "VertexArrayObject.h"

class Mesh;

class MeshBuffers{

private:
	VertexArrayObject* vaoObject;
	
	VertexBufferObject* vertexBufferObject;
	VertexBufferObject* textureCoordsBufferObject;
	VertexBufferObject* normalBufferObject;
	VertexBufferObject* tangentBufferObject;
	VertexBufferObject* binormalBufferObject;	
	IndexBufferObject* indexBufferObject;
	VertexBufferObject* jointIndexBufferObject;
	VertexBufferObject* jointWeightBufferObject;

	float* vertexBuffer;
	float* textureCoords;
	float* normalBuffer;
	float* tangentBuffer;
	float* binormalBuffer;
	unsigned int* indexBuffer;
	unsigned short* jointIndexBuffer;
	float* jointWeightBuffer;

	unsigned int vertexBufferSize;
	unsigned int textureBufferSize;
	unsigned int normalBufferSize;
	unsigned int tangentBufferSize;
	unsigned int binormalBufferSize;
	unsigned int indexBufferSize;
	unsigned int jointIndexBufferSize;
	unsigned int jointWeightBufferSize;

	bool alreadyLoaded;

public:
	MeshBuffers();
    virtual ~MeshBuffers();
    
	unsigned int getVaoHandle(){ return this->vaoObject->getHandler(); }
	VertexArrayObject* getVaoObject(){ return this->vaoObject; }
	void setVaoObject(VertexArrayObject* vao){ this->vaoObject = vao; }

	VertexBufferObject* getVertexBufferObject(){ return this->vertexBufferObject; }	
	VertexBufferObject* getTextureCoordsBufferObject(){ return this->textureCoordsBufferObject; }	
	VertexBufferObject* getNormalBufferObject(){ return this->normalBufferObject; }	
	VertexBufferObject* getTangentBufferObject(){ return this->tangentBufferObject; }	
	VertexBufferObject* getBinormalBufferObject(){ return this->binormalBufferObject; }	
	IndexBufferObject* getIndexBufferObject(){ return this->indexBufferObject; }
	VertexBufferObject* getJointIndexBufferObject(){ return this->jointIndexBufferObject; }
	VertexBufferObject* getJointWeightBufferObject(){ return this->jointWeightBufferObject; }	

	void setVertexBufferObject(VertexBufferObject* vbo){ this->vertexBufferObject = vbo; }	
	void setNormalBufferObject(VertexBufferObject* vbo){ this->normalBufferObject = vbo; }	
	void setTangentBufferObject(VertexBufferObject* vbo){ this->tangentBufferObject = vbo; }	
	void setBinormalBufferObject(VertexBufferObject* vbo){ this->binormalBufferObject = vbo; }	
	void setTextureCoordsBufferObject(VertexBufferObject* vbo){ this->textureCoordsBufferObject = vbo; }	
	void setIndexBufferObject(IndexBufferObject* vbo){ this->indexBufferObject = vbo; }	
	void setJointIndexBufferObject(VertexBufferObject* vbo){ this->jointIndexBufferObject = vbo; }	
	void setJointWeightBufferObject(VertexBufferObject* vbo){ this->jointWeightBufferObject = vbo; }	
	
	unsigned int* getIndexBuffer(){ return this->indexBuffer; }
	unsigned int getIndexBufferSize(){ return this->indexBufferSize; }
	float* getVertexBuffer(){ return this->vertexBuffer; }

	void setVertexBuffer(float* buffer, unsigned int size){ this->vertexBuffer = buffer; this->vertexBufferSize = size;}
	void setTextureCoords(float* buffer, unsigned int size){ this->textureCoords = buffer; this->textureBufferSize = size; }
	void setNormalBuffer(float* buffer, unsigned int size){ this->normalBuffer = buffer; this->normalBufferSize = size; }
	void setTangentBuffer(float* buffer, unsigned int size){ this->tangentBuffer = buffer; this->tangentBufferSize = size; }
	void setBinormalBuffer(float* buffer, unsigned int size){ this->binormalBuffer = buffer; this->binormalBufferSize = size; }
	void setIndexBuffer(unsigned int* buffer, unsigned int size){ this->indexBuffer = buffer; this->indexBufferSize = size; }
	void setJointIndexBuffer(unsigned short* buffer, unsigned int size){ this->jointIndexBuffer = buffer; this->jointIndexBufferSize = size; }
	void setJointWeightBuffer(float* buffer, unsigned int size){ this->jointWeightBuffer = buffer; this->jointWeightBufferSize = size;}
	
	// Indica si los buffers fueron cargados en memoria
	void setAlreadyLoaded(bool value){ this->alreadyLoaded = value; }
	bool isAlreadyLoaded(){ return this->alreadyLoaded; }
	
	// Carga en memoria el meshBuffer (solo aquellos buffers que estan completos)
	void load();

	// Clona el meshBuffer actual, conservando las referencias a memoria de video del original (para ahorrar memoria)
	void clone(MeshBuffers* clone);
	// Clona solo la data en bruto del meshBuffer actual, pero sin realizar la carga a memoria de video (util para separar buffers de memoria ej. para trasformacines)
	void cloneUninitialized(MeshBuffers* clone);
};
