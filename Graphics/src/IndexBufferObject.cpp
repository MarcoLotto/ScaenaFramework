/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "IndexBufferObject.h"
#include "GraphicDevice.h"

void IndexBufferObject::initiateInMemory(){
	this->handler = GraphicDevice::getInstance()->generateVboBuffer();
}

IndexBufferObject::IndexBufferObject(unsigned int* bufferData, unsigned int bufferDataLenght) : DataBuffer(){
	this->handler = 0;
	this->bufferData = bufferData;
	this->bufferDataLenght = bufferDataLenght;
	GraphicDevice* device = GraphicDevice::getInstance();	
	this->drawMethod = device->getConstantCatalog()->getStaticDrawFlag();
	this->dataType = device->getConstantCatalog()->getUnsignedIntTypeFlag();	
}

IndexBufferObject::~IndexBufferObject(){
	if(this->isAlreadyLoaded())
		GraphicDevice::getInstance()->deleteVboBuffer(this->handler);
}

void IndexBufferObject::load(VertexArrayObject* vao){
	if(!this->alreadyLoaded){
		this->alreadyLoaded = true;
		// Primero bindeo el VAO (antes de crear el VBO)
		vao->bind();

		// Ahora creo el VBO y le cargo el contenido
		this->initiateInMemory();
		GraphicIndexArrayContainer container = GraphicIndexArrayContainer(this->handler, this->bufferDataLenght, this->bufferData, this->drawMethod, this->dataType, vao); 
		GraphicDevice::getInstance()->fillVboWithData(&container);		

		// Desvindeo el vao del contexto
		vao->unbind();
	}
}

IndexBufferObject* IndexBufferObject::clone(){
	IndexBufferObject* clone = new IndexBufferObject(this->bufferData, this->bufferDataLenght);
	clone->setDataType(this->dataType);
	clone->setAlreadyLoaded(this->alreadyLoaded);
	clone->setDrawMethod(this->drawMethod);	
	clone->setHandler(this->handler);
	return clone;
}