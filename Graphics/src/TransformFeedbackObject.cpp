/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "TransformFeedbackObject.h"
#include "GraphicDevice.h"

void TransformFeedbackObject::initiateInMemory(){
	this->handler = GraphicDevice::getInstance()->generateTransformFeedback();
}

TransformFeedbackObject::TransformFeedbackObject() : DataBuffer(){
	this->handler = 0;
	this->attachedVertexBufferObject = new list<VertexBufferObject*>();	
}

TransformFeedbackObject::~TransformFeedbackObject(){
	delete this->attachedVertexBufferObject;

	// Borro el buffer de la memoria de video
	if(this->isAlreadyLoaded())
		GraphicDevice::getInstance()->deleteTransformFeedback(this->handler);
}

// Agrega un nuevo VBO (tambien se asocia en memoria de video)
void TransformFeedbackObject::attachVertexBufferObject(VertexBufferObject* vbo){
	this->attachedVertexBufferObject->push_back(vbo);

	// Si es la primera vez que hago un attach, creo el handler
	if(!this->isAlreadyLoaded()){
		this->initiateInMemory();
		this->alreadyLoaded = true;
	}
	// Attach fisico del vbo al transform feedback
	unsigned int attachIndex = this->attachedVertexBufferObject->size() - 1;
	GraphicDevice::getInstance()->attachVboToTransformFeedback(this->handler, vbo->getHandler(), attachIndex); 
}

TransformFeedbackObject* TransformFeedbackObject::clone(){
	TransformFeedbackObject* clone = new TransformFeedbackObject();
	clone->setHandler(this->handler);
	clone->setAlreadyLoaded(this->alreadyLoaded);	
	list<VertexBufferObject*>::iterator it = this->attachedVertexBufferObject->begin();
	while(it != this->attachedVertexBufferObject->end()){
		clone->getAttachedVertexBufferObjects()->push_back(*it);
		++it;
	}
	return clone;
}