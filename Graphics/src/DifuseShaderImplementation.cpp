/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "DifuseShaderImplementation.h"
#include "Mesh.h"

void DifuseShaderImplementation::prepareForDraw(Mesh* mesh){
		
	//Activo el programa de shaders
	this->use();
	DrawPacket* drawPacket = mesh->getDrawPacket();

	//Cargo matrices de modelo, view, proyeccion y normal
	this->vmMatrix = drawPacket->getVmMatrix();
	this->setUniform(this->pvmMatrixUniform, drawPacket->getPvmMatrix());	
	this->setUniform(this->normalMatrixUniform, drawPacket->getNormalMatrix());

	//Asigno la textura
	mesh->getDiffuseMap()->chargeIntoShader(this, 0);

	//Asigno los materiales (por ahora para todos igual)
	mesh->getMaterial()->chargeIntoShader(this, 0);

	//Asigno las transformaciones UV
	mesh->getUVTransformations()->chargeIntoShader(this, 0);

	// Si los hijos quieren agregar algo mas al dibujado, ahora es el momento
	this->extendedDraw(mesh);
}
