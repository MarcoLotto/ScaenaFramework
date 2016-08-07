/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ParallaxShaderImplementation.h"
#include "NormalMapShader.h"
#include "Mesh.h"

//***PARAMETROS GENERALES*********************************************************************************
#define TO_LOWERSHADER_DISTANCE 20.0f //Se define la distancia para que cambie el shader a uno mas básico
//********************************************************************************************************

void ParallaxShaderImplementation::prepareForDraw(Mesh* mesh){		
	//Activo el programa de shaders
	this->use();
	DrawPacket* drawPacket = mesh->getDrawPacket();
	
	//Cargo matrices de modelo, view, proyeccion y normal	
	this->setUniform(this->vmMatrixUniform, drawPacket->getVmMatrix());
	this->setUniform(this->pvmMatrixUniform, drawPacket->getPvmMatrix());	
	this->setUniform(this->normalMatrixUniform, drawPacket->getNormalMatrix());

	//Asigno la textura
	mesh->getDiffuseMap()->chargeIntoShader(this, 0);
	//Asigno el normal map
	mesh->getNormalMap()->chargeIntoShader(this, 1);
	//Asigno el displacement map
	mesh->getDisplacementMap()->chargeIntoShader(this, 2);
	
	//Asigno el factor de decaimiento
	float factorDeDecaimiento = (TO_LOWERSHADER_DISTANCE - this->getDistanceFactor())/TO_LOWERSHADER_DISTANCE;
	//Seteo el factor de decaimiento para modificar dentro del shader el factor de decaimiento y normal
	this->setUniform(this->atenuationFactorUniform, factorDeDecaimiento);
		
	//Asigno los materiales (por ahora para todos igual)
	mesh->getMaterial()->chargeIntoShader(this, 0);

	//Asigno las transformaciones UV
	mesh->getUVTransformations()->chargeIntoShader(this, 0);

	// Si los hijos quieren agregar algo mas al dibujado, ahora es el momento
	this->extendedDraw(mesh);
}

void ParallaxShaderImplementation::initOptimizationsParameters(){
	// Se setean los parametros de optimizacion del shader
	this->setLowerShader(NormalMapShader::getInstance()); //A que shader pasa al optimizar
	this->setDistanceToChangeShader(TO_LOWERSHADER_DISTANCE); //Distancia en la que se optimizará
}