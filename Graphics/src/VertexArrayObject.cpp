/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "VertexArrayObject.h"
#include "GraphicDevice.h"

void VertexArrayObject::initiateInMemory(){
	this->handler = GraphicDevice::getInstance()->generateVaoBuffer();
}

VertexArrayObject::VertexArrayObject() : DataBuffer(){
	this->handler = 0;
	this->attachementsCount = 0;
}

VertexArrayObject::~VertexArrayObject(){
	// Borro el buffer de la memoria de video
	if(this->isAlreadyLoaded())
		GraphicDevice::getInstance()->deleteVaoBuffer(this->handler);
}

void VertexArrayObject::bind(){	
	// Si es la primera vez que hago un attach, creo el handler
	if(!this->isAlreadyLoaded()){
		this->initiateInMemory();
		this->alreadyLoaded = true;
	}	
	GraphicDevice::getInstance()->bindVaoBuffer(this->handler); 
}

void VertexArrayObject::unbind(){	
	GraphicDevice::getInstance()->bindVaoBuffer(0); 
}

VertexArrayObject* VertexArrayObject::clone(){
	VertexArrayObject* clone = new VertexArrayObject();
	clone->setHandler(this->handler);
	clone->setAlreadyLoaded(this->alreadyLoaded);	
	clone->setAttachementsCount(this->attachementsCount);
	return clone;
}

void VertexArrayObject::addVboAttachement(VertexBufferObject* vbo){
	this->bind();
	GraphicDevice::getInstance()->attachVboToVao(vbo, this->attachementsCount);
	this->addAttachementCount();
	this->unbind();
}
