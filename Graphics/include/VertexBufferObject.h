/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "VertexArrayObject.h"
#include "DataBuffer.h"

// Es el objecto que reprensenta la información de vertices en memoria de video.
class VertexBufferObject : public DataBuffer{

private:
	void* bufferData;
	unsigned int bufferDataLenght;
	unsigned int drawMethod;
	unsigned int dataType;
	unsigned int componentCount;

protected:
	void initiateInMemory();

public:	
	VertexBufferObject(float* bufferData, unsigned int bufferDataLenght, unsigned int componentCount);
	VertexBufferObject(unsigned int* bufferData, unsigned int bufferDataLenght, unsigned int componentCount);
    VertexBufferObject(unsigned short* bufferData, unsigned int bufferDataLenght, unsigned int componentCount);
	virtual ~VertexBufferObject();

	void setDrawMethod(unsigned int drawMethod){ this->drawMethod = drawMethod; }
	void setDataType(unsigned int type){ this->dataType = type; }
	void setComponentCount(unsigned int count){ this->componentCount = count; }

	unsigned int getDrawMethod(){ return this->drawMethod; }
	unsigned int getDataType(){ return this->dataType; }
	unsigned int getComponentCount(){ return this->componentCount; }	

	void* getBufferData(){ return this->bufferData; }
	unsigned int getBufferDataLenght(){ return this->bufferDataLenght; }	

	void load(VertexArrayObject* vao);
	VertexBufferObject* clone();
};