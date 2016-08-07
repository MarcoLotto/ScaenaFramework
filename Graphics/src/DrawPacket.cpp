/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "DrawPacket.h"

// Calcula a partir de las tres matrices de modelo, proyeccion y vista, las convinaciones de las mismas y la matriz normal
void DrawPacket::calculateMatrixDrawData(){
	
	// Calculo las convinaciones de las tres matrices
	this->vmMatrix = this->viewMatrix * this->modelMatrix;	
	this->pvmMatrix = this->projMatrix * this->vmMatrix;	
	
	//Armo la matriz normal
	for(int i=0; i < 3; i++){
		for(int j=0; j < 3 ; j++){
			this->normalMatrix[i][j] = this->vmMatrix[i][j];
		}
	}
	this->normalMatrix = glm::inverse(this->normalMatrix);
	this->normalMatrix = glm::transpose(this->normalMatrix);
}