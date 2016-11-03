/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "DataBuffer.h"

class VertexBufferObject;

// Un VAO se utiliza para dibujar varios VBO en un solo batch.
class VertexArrayObject : public DataBuffer{

private:	
	unsigned int attachementsCount;

protected:
	void initiateInMemory();

public:	
	VertexArrayObject();
	virtual ~VertexArrayObject();

	// Bindea y desvindea el VAO al contexto
	void bind();
	void unbind();

	// Clona el VAO, manteniendo el mismo handler (no reserva mas memoria en video)
	VertexArrayObject* clone();

	// Indica la cantidad de VBOs attacheados a este VAO
	unsigned int getAttachementsCount(){ return this->attachementsCount; }
	void setAttachementsCount(unsigned int count){ this->attachementsCount = count; }
	void addAttachementCount(){ this->attachementsCount++; }
	void addVboAttachement(VertexBufferObject* vbo);
};