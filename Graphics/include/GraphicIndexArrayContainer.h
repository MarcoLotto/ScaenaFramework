/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "VertexArrayObject.h"

// Esta clase sirve para realizar requests de carga en memoria de video de un array buffer.
// Al finalizar la carga se guarda en el atributo vboHandle el handle de object buffer correspondiente.
class GraphicIndexArrayContainer{

private:
	unsigned int bufferSize;
	unsigned int* buffer;
	unsigned int drawMethod;
	unsigned int dataType;
	unsigned int vboHandle;
	VertexArrayObject* vao;

public:
	GraphicIndexArrayContainer(unsigned int vboHandle, unsigned int bufferSize, unsigned int* buffer, unsigned int drawMethod, unsigned int dataType, VertexArrayObject* vao){
		this->vboHandle = vboHandle;
		this->buffer = buffer;
		this->bufferSize = bufferSize;
		this->drawMethod = drawMethod;
		this->dataType = dataType;
		this->vao = vao;
	};

	unsigned int getBufferSize(){ return this->bufferSize; }
	unsigned int* getBuffer(){ return this->buffer; }
	unsigned int getDrawMethod(){ return this->drawMethod; } 
	unsigned int getDataType(){ return this->dataType; }
	unsigned int getVboHandle(){ return this->vboHandle; }
	void setVboHandle(unsigned int handle){ this->vboHandle = handle; }
	VertexArrayObject* getVao(){ return this->vao; }
};