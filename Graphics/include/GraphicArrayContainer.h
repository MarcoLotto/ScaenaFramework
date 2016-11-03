/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "VertexArrayObject.h"

// Esta clase sirve para realizar requests de carga en memoria de video de un buffer.
// Al finalizar la carga se guarda en el atributo vboHandle el handle de object buffer correspondiente.
class GraphicArrayContainer{

private:
	unsigned int bufferSize;
	void* buffer;
	unsigned int drawMethod;
	unsigned int dataType;
	unsigned int componentCount;
	unsigned int vboHandle;
	VertexArrayObject* vao;

public:
	GraphicArrayContainer(unsigned int vboHandle, unsigned int bufferSize, void* buffer, unsigned int drawMethod, unsigned int dataType, unsigned int componentCount, VertexArrayObject* vao){
		this->buffer = buffer;
		this->bufferSize = bufferSize;
		this->drawMethod = drawMethod;
		this->dataType = dataType;
		this->componentCount = componentCount;
		this->vboHandle = vboHandle;
		this->vao = vao;
	};

	unsigned int getBufferSize(){ return this->bufferSize; }
	void* getBuffer(){ return this->buffer; }
	unsigned int getDrawMethod(){ return this->drawMethod; } 
	unsigned int getDataType(){ return this->dataType; }
	unsigned int getComponentCount(){ return this->componentCount; }
	unsigned int getVboHandle(){ return this->vboHandle; }
	void setVboHandle(unsigned int handle){ this->vboHandle = handle; }
	VertexArrayObject* getVao(){ return this->vao; }
};