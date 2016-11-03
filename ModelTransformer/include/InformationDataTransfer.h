/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
typedef glm::vec3 vec3;
typedef glm::mat4 mat4;

class InformationDataTransfer{

private:

public:
	vec3 position;
	vec3 rotation;
	vec3 scale;
	mat4 transformationMatrix;
	mat4 inverseTransformationMatrix;

	void generateTransformationMatrix(){
		this->transformationMatrix = glm::translate(mat4(1.0f), this->position);
		this->transformationMatrix = glm::rotate(this->transformationMatrix, this->rotation.z, vec3(0.0f, 0.0f, 1.0f));
		this->transformationMatrix = glm::rotate(this->transformationMatrix, this->rotation.y, vec3(0.0f, 1.0f, 0.0f));
		this->transformationMatrix = glm::rotate(this->transformationMatrix, this->rotation.x, vec3(1.0f, 0.0f, 0.0f));		
		this->transformationMatrix = glm::scale(this->transformationMatrix, this->scale);		
		this->inverseTransformationMatrix = glm::inverse(this->transformationMatrix);
	}

};