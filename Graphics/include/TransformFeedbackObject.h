/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "VertexBufferObject.h"
#include <list>
using namespace std;

class TransformFeedbackObject : public DataBuffer{

private:
	list<VertexBufferObject*>* attachedVertexBufferObject;

protected:
	void initiateInMemory();

public:	
	TransformFeedbackObject();
	virtual ~TransformFeedbackObject();

	// Agrega un nuevo VBO (tambien se asocia en memoria de video)
	void attachVertexBufferObject(VertexBufferObject* vbo);
	TransformFeedbackObject* clone();

	list<VertexBufferObject*>* getAttachedVertexBufferObjects(){ return this->attachedVertexBufferObject; }
};