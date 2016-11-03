/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "Transformation.h"
#include "Mesh.h"

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform2.hpp> 
#include <glm/gtx/projection.hpp>

Transformation::Transformation(){
	this->localRecalculationNeeded = true;
	this->position = vec3(0.0f);
	this->rotation = vec3(0.0f);
	this->scale = vec3(1.0f);
}

// Devuelve la matrix de modelo global, lista para usar en el shader. Recibe la matriz
// global del mesh padre y un indicador de si esta fue cambiada desde el ultimo frame. Indica si los hijos tienen que recalcularse.
mat4 Transformation::getGlobalModelMatrix(mat4 parentGlobalModelMatrix, bool needRecalculation, bool* childsRecalculation){
	if(needRecalculation || this->localRecalculationNeeded){
		(*childsRecalculation) = true;
		this->globalModelMatrix = parentGlobalModelMatrix * this->getLocalModelMatrix();
		this->parentModelMatrix = parentGlobalModelMatrix;
	}
	else{
		(*childsRecalculation) = false;
	}
	return this->globalModelMatrix;
}

// Devuelve la matriz de modelo local. Es recalculada solo si es necesario.
mat4 Transformation::getLocalModelMatrix(){
	if(this->localRecalculationNeeded){
		// Recalculo la matriz local
		this->localModelMatrix = glm::translate(mat4(1.0f), this->position);
		this->localModelMatrix = glm::rotate(this->localModelMatrix, this->rotation.z, vec3(0.0f, 0.0f, 1.0f));
		this->localModelMatrix = glm::rotate(this->localModelMatrix, this->rotation.y, vec3(0.0f, 1.0f, 0.0f));
		this->localModelMatrix = glm::rotate(this->localModelMatrix, this->rotation.x, vec3(1.0f, 0.0f, 0.0f));
		this->localModelMatrix = glm::scale(this->localModelMatrix, this->scale);
		this->localRecalculationNeeded = false;
	}
	return this->localModelMatrix;
}

void Transformation::clone(Transformation* clone){
	// No preciso copiar mas datos, porque se van a recalcular solos
	clone->setPosition(this->position);
	clone->setRotation(this->rotation);
	clone->setScale(this->scale);
}

void chargeIntoShader(GLSLProgram* shader, unsigned int textureUnit){
	// TODO
}

void Transformation::setPosition(vec3 position){
	this->position = position;
	this->localRecalculationNeeded = true;
}
void Transformation::setRotation(vec3 rotation){
	this->rotation = rotation;
	this->localRecalculationNeeded = true;
}
void Transformation::setScale(vec3 scale){
	this->scale = scale;
	this->localRecalculationNeeded = true;
}

vec3 Transformation::getPosition(){
	return this->position;
}
vec3 Transformation::getRotation(){
	return this->rotation;
}
vec3 Transformation::getScale(){
	return this->scale;
}

// Indica si la matriz del padre ha cambiado
bool Transformation::hasParentModelMatrixChanged(mat4 newParentMatrix){
	// Desdoblo el for para intentar aumentar algo de performance
	if(this->parentModelMatrix[0][0] != newParentMatrix[0][0]) return true;
	if(this->parentModelMatrix[0][1] != newParentMatrix[0][1]) return true;
	if(this->parentModelMatrix[0][2] != newParentMatrix[0][2]) return true;
	if(this->parentModelMatrix[0][3] != newParentMatrix[0][3]) return true;
	if(this->parentModelMatrix[1][0] != newParentMatrix[1][0]) return true;
	if(this->parentModelMatrix[1][1] != newParentMatrix[1][1]) return true;
	if(this->parentModelMatrix[1][2] != newParentMatrix[1][2]) return true;
	if(this->parentModelMatrix[1][3] != newParentMatrix[1][3]) return true;
	if(this->parentModelMatrix[2][0] != newParentMatrix[2][0]) return true;
	if(this->parentModelMatrix[2][1] != newParentMatrix[2][1]) return true;
	if(this->parentModelMatrix[2][2] != newParentMatrix[2][2]) return true;
	if(this->parentModelMatrix[2][3] != newParentMatrix[2][3]) return true;
	if(this->parentModelMatrix[3][0] != newParentMatrix[3][0]) return true;
	if(this->parentModelMatrix[3][1] != newParentMatrix[3][1]) return true;
	if(this->parentModelMatrix[3][2] != newParentMatrix[3][2]) return true;
	if(this->parentModelMatrix[3][3] != newParentMatrix[3][3]) return true;
	return false;
}