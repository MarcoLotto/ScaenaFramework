/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "VertexArrayObject.h"
#include "DataBuffer.h"

// Es el objecto que reprensenta la información de indices (o element arrays) en memoria de video.
class IndexBufferObject : public DataBuffer{

private:
	unsigned int* bufferData;
	unsigned int bufferDataLenght;
	unsigned int drawMethod;
	unsigned int dataType;
	unsigned int componentCount;

protected:
	void initiateInMemory();

public:	
	IndexBufferObject(unsigned int* bufferData, unsigned int bufferDataLenght);
	virtual ~IndexBufferObject();

	void setDrawMethod(unsigned int drawMethod){ this->drawMethod = drawMethod; }
	void setDataType(unsigned int type){ this->dataType = type; }	

	unsigned int getDrawMethod(){ return this->drawMethod; }
	unsigned int getDataType(){ return this->dataType; }	

	void load(VertexArrayObject* vao);
	IndexBufferObject* clone();
};