/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "VertexBufferObject.h"
#include "GraphicDevice.h"
#include "ApiVariableTypeDoesNotExistException.h"

void VertexBufferObject::initiateInMemory(){
	this->handler = GraphicDevice::getInstance()->generateVboBuffer();
}

VertexBufferObject::VertexBufferObject(float* bufferData, unsigned int bufferDataLenght, unsigned int componentCount) : DataBuffer(){
	this->handler = 0;
	this->bufferData = bufferData;
	this->bufferDataLenght = bufferDataLenght;	
	GraphicDevice* device = GraphicDevice::getInstance();	
	this->drawMethod = device->getConstantCatalog()->getStaticDrawFlag();
	this->dataType = device->getConstantCatalog()->getFloatTypeFlag();
	this->componentCount = componentCount;
}

VertexBufferObject::VertexBufferObject(unsigned int* bufferData, unsigned int bufferDataLenght, unsigned int componentCount) : DataBuffer(){
	this->handler = 0;
	this->bufferData = bufferData;
	this->bufferDataLenght = bufferDataLenght;	
	GraphicDevice* device = GraphicDevice::getInstance();	
	this->drawMethod = device->getConstantCatalog()->getStaticDrawFlag();
	this->dataType = device->getConstantCatalog()->getUnsignedIntTypeFlag();
	this->componentCount = componentCount;
}

VertexBufferObject::VertexBufferObject(unsigned short* bufferData, unsigned int bufferDataLenght, unsigned int componentCount) : DataBuffer(){
    this->handler = 0;
    this->bufferData = bufferData;
    this->bufferDataLenght = bufferDataLenght;
    GraphicDevice* device = GraphicDevice::getInstance();
    this->drawMethod = device->getConstantCatalog()->getStaticDrawFlag();
    this->dataType = device->getConstantCatalog()->getUnsignedShortTypeFlag();
    this->componentCount = componentCount;
}

VertexBufferObject::~VertexBufferObject(){
	if(this->isAlreadyLoaded())
		GraphicDevice::getInstance()->deleteVboBuffer(this->handler);
}

void VertexBufferObject::load(VertexArrayObject* vao){
	if(!this->alreadyLoaded){
		this->alreadyLoaded = true;
		// Primero bindeo el VAO (antes de crear el VBO)
		vao->bind();

		// Ahora creo el VBO y le cargo el contenido
		this->initiateInMemory();
		GraphicArrayContainer container = GraphicArrayContainer(this->handler, this->bufferDataLenght, this->bufferData, this->drawMethod, this->dataType, this->componentCount, vao); 
		GraphicDevice::getInstance()->fillVboWithData(&container);

		// Desvindeo el vao del contexto
		vao->unbind();
	}
}

VertexBufferObject* VertexBufferObject::clone(){
	VertexBufferObject* clone = NULL;
	if(this->dataType == GraphicDevice::getInstance()->getConstantCatalog()->getFloatTypeFlag()){
		clone = new VertexBufferObject((float*) this->bufferData, this->bufferDataLenght, this->componentCount);
	}
	else if(this->dataType == GraphicDevice::getInstance()->getConstantCatalog()->getUnsignedIntTypeFlag()){
		clone = new VertexBufferObject((unsigned int*) this->bufferData, this->bufferDataLenght, this->componentCount);
	}
    else if(this->dataType == GraphicDevice::getInstance()->getConstantCatalog()->getUnsignedShortTypeFlag()){
        clone = new VertexBufferObject((unsigned short*) this->bufferData, this->bufferDataLenght, this->componentCount);
    }
	else{
		throw new ApiVariableTypeDoesNotExistException("El tipo de data especificado para el vertex buffer object no es correcto");
	}
	clone->setDataType(this->dataType);
	clone->setAlreadyLoaded(this->alreadyLoaded);
	clone->setDrawMethod(this->drawMethod);	
	clone->setHandler(this->handler);
	return clone;
}