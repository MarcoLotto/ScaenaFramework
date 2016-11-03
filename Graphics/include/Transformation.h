/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ChargeableResource.h"
#include <glm/glm.hpp> 

class Mesh;

typedef glm::vec3 vec3;
typedef glm::mat4 mat4;

class Transformation{

protected:
	mat4 localModelMatrix;
	mat4 globalModelMatrix;
	mat4 parentModelMatrix;
	vec3 position;
	vec3 rotation;
	vec3 scale;
	bool localRecalculationNeeded;
		
public:
	Transformation();

	// Devuelve la matrix de modelo global, lista para usar en el shader. Recibe la matriz
	// global del mesh padre y un indicador de si esta fue cambiada desde el ultimo frame. Indica si los hijos tienen que recalcularse.
	mat4 getGlobalModelMatrix(mat4 parentGlobalModelMatrix, bool needRecalculation, bool* childsRecalculation);

	// Devuelve la matriz de modelo local. Es recalculada solo si es necesario.
	mat4 getLocalModelMatrix();

	// Indica si la matriz del padre ha cambiado
	bool hasParentModelMatrixChanged(mat4 newParentMatrix);
	
	virtual void setPosition(vec3 position);
	virtual void setRotation(vec3 rotation);
	virtual void setScale(vec3 scale);

	void chargeIntoShader(GLSLProgram* shader, unsigned int textureUnit);
	void clone(Transformation* clone);

	vec3 getPosition();
	vec3 getRotation();
	vec3 getScale();


};