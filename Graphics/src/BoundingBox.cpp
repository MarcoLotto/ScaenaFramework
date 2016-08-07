/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "BoundingBox.h"

// Dado un vertex buffer, recalcula el bounding box
void BoundingBox::recalculate(VertexBufferObject* vbo){
	this->minPoint = vec3(0.0f);
	this->maxPoint = vec3(0.0f);

	// Me aseguro que el vbo sea valido
	if(vbo == NULL) return;

	// Analizo el vertexbuffer y consigo los puntos de minimo y maximo del bounding box
	float* vertexBuffer = (float*) vbo->getBufferData();
	for(unsigned int i=0; i < vbo->getBufferDataLenght(); i+=3){
		if(vertexBuffer[i] < this->minPoint.x) this->minPoint.x = vertexBuffer[i];
		if(vertexBuffer[i+1] < this->minPoint.y) this->minPoint.y = vertexBuffer[i+1];
		if(vertexBuffer[i+2] < this->minPoint.z) this->minPoint.z = vertexBuffer[i+2];

		if(vertexBuffer[i] > this->maxPoint.x) this->maxPoint.x = vertexBuffer[i];
		if(vertexBuffer[i+1] > this->maxPoint.y) this->maxPoint.y = vertexBuffer[i+1];
		if(vertexBuffer[i+2] > this->maxPoint.z) this->maxPoint.z = vertexBuffer[i+2];
	}
}

void BoundingBox::updateTransformationMatrix(mat4 modelMatrix){ 
	this->absMinPoint = vec3(modelMatrix * vec4(this->minPoint, 1.0f));
	this->absMaxPoint = vec3(modelMatrix * vec4(this->maxPoint, 1.0f));
}

// Devuelve true si colisionan ambos bounding box
bool BoundingBox::isColliding(BoundingBox* otherBounding){
	//******************************************************************************************************************************
	// REVIEW: Esto para probar y simplificar realiza una comprobacion de colision como si fuese un bounding sphere, cambiar esto!!!
	//******************************************************************************************************************************

	// Si no tengo informacion de bounding aun, no proceso la colision
	if(this->getAbsoluteMaxPoint() == vec3(0.0f) && this->getAbsoluteMaxPoint() == vec3(0.0f)){
		return false;
	}

	// Primero calculo y aproximo los valores para un bounding sphere a partir del bounding box
	vec3 absBB1CenterPoint = (this->getAbsoluteMaxPoint() + this->getAbsoluteMinPoint()) / 2.0f;
	vec3 absBB2CenterPoint = (otherBounding->getAbsoluteMaxPoint() + otherBounding->getAbsoluteMinPoint()) / 2.0f;
	float aproxBB1Radius = glm::distance(this->getAbsoluteMaxPoint(), this->getAbsoluteMinPoint()) / 2.0f;
	float aproxBB2Radius = glm::distance(otherBounding->getAbsoluteMaxPoint(), otherBounding->getAbsoluteMinPoint()) / 2.0f;

	// Ahora verifico si ambos colisionan
	return (glm::distance(absBB1CenterPoint, absBB2CenterPoint) <= aproxBB1Radius + aproxBB2Radius);
}

// Devuelve true si colisiona el mesh con la recta
bool BoundingBox::isIntersecting(LineR3* line){
	//******************************************************************************************************************************
	// REVIEW: Esto para probar y simplificar realiza una comprobacion de colision como si fuese un bounding sphere, cambiar esto!!!
	//******************************************************************************************************************************

	// Si no tengo informacion de bounding aun, no proceso la colision
	if(this->getAbsoluteMaxPoint() == vec3(0.0f) && this->getAbsoluteMaxPoint() == vec3(0.0f)){
		return false;
	}

	// Primero calculo y aproximo los valores para un bounding sphere a partir del bounding box
	vec3 absBB1CenterPoint = (this->getAbsoluteMaxPoint() + this->getAbsoluteMinPoint()) / 2.0f;
	float aproxBB1Radius = glm::distance(this->getAbsoluteMaxPoint(), this->getAbsoluteMinPoint()) / 2.0f;

	// Encuentro la distancia minima dentro de la recta, del centro del bounding box
	float minimalDistancePoint = line->getMinimalDistanceToPoint(absBB1CenterPoint);

	// Ahora verifico si colisiona
	return (minimalDistancePoint <= aproxBB1Radius);
}

// Dado un boundingbox clon (con memoria ya asignada) copia los valores de este en el clon
void BoundingBox::clone(BoundingBox* clone){
	clone->setAbsoluteMaxPoint(this->absMaxPoint);
	clone->setAbsoluteMinPoint(this->absMinPoint);
	clone->setLocalMaxPoint(this->maxPoint);
	clone->setLocalMinPoint(this->minPoint);
}