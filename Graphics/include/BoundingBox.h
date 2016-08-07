/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "VertexBufferObject.h"
#include "LineR3.h"
#include <glm/glm.hpp> 
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat4 mat4;

class BoundingBox{

protected:
	vec3 minPoint, maxPoint, absMinPoint, absMaxPoint;

public:
	// Dado un vertex buffer, recalcula el bounding box
	void recalculate(VertexBufferObject* vbo);

	// Devuelve true si colisionan ambos bounding box
	bool isColliding(BoundingBox* otherBounding);
	// Devuelve true si el bounding intersecta con la recta
	bool isIntersecting(LineR3* line);

	// Dado un boundingbox clon (con memoria ya asignada) copia los valores de este en el clon
	void clone(BoundingBox* clone);

	// Se debe llamar cada vez que cambie la matriz de transformacion del mesh u objeto
	void updateTransformationMatrix(mat4 modelMatrix);

	// Se consigue los puntos del bounding box en su posicion absoluta en la escena
	vec3 getAbsoluteMinPoint(){ return this->absMinPoint; }
	vec3 getAbsoluteMaxPoint(){ return this->absMaxPoint; }

	// Se consigue los puntos del bounding box sin aplicar transformaciones
	vec3 getLocalMinPoint(){ return this->minPoint; }
	vec3 getLocalMaxPoint(){ return this->maxPoint; }

	// Setters
	void setLocalMinPoint(vec3 value){ this->minPoint = value; }
	void setLocalMaxPoint(vec3 value){ this->maxPoint = value; }
	void setAbsoluteMinPoint(vec3 value){ this->absMinPoint = value; }
	void setAbsoluteMaxPoint(vec3 value){ this->absMaxPoint = value; }
};