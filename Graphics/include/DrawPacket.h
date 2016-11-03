/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <glm/glm.hpp>
typedef glm::mat4 mat4;
typedef glm::mat3 mat3;

class DrawPacket{

private:
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 vmMatrix;
	mat4 pvmMatrix;	
	mat3 normalMatrix;
	
public:
	void setModelMatrix(mat4 matrix){ this->modelMatrix = matrix; } 
	void setViewMatrix(mat4 matrix){ this->viewMatrix = matrix; } 
	void setProjMatrix(mat4 matrix){ this->projMatrix = matrix; } 

	mat4 getModelMatrix(){ return this->modelMatrix; } 
	mat4 getViewMatrix(){ return this->viewMatrix; } 
	mat4 getProjMatrix(){ return this->projMatrix; } 

	// Calcula a partir de las tres matrices de modelo, proyeccion y vista, las convinaciones de las mismas y la matriz normal
	void calculateMatrixDrawData();

	mat4 getVmMatrix(){ return this->vmMatrix; } 
	mat4 getPvmMatrix(){ return this->pvmMatrix; } 
	mat3 getNormalMatrix(){ return this->normalMatrix; } 
};