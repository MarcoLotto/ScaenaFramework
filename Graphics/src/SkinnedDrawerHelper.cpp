/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SkinnedDrawerHelper.h"
#include "SkinnedMesh.h"
#include "StringUtils.h"

void SkinnedDrawerHelper::skinAttributesInit(GLSLProgram* glslProgram, list<string>* inputAttributes){
	inputAttributes->push_back("VertexJointIndexes");
	inputAttributes->push_back("VertexJointWeights");	
}

void SkinnedDrawerHelper::skinUniformsInit(GLSLProgram* glslProgram, GLSLUniform** jointsTransformUniform){
		(*jointsTransformUniform) = new GLSLUniform("jointsTransformUniform", glslProgram);
}

void SkinnedDrawerHelper::skinDraw(GLSLProgram* glslProgram, Mesh* mesh, GLSLUniform* jointsTransformUniform){
	// Si llego acá tiene que (espero) ser un skinned mesh
	SkinnedMesh* skinnedMesh = (SkinnedMesh*) mesh;

	// Consigo las transformaciones finales de los joints, las cuales se terminaran aplicando a los vertices
	int jointsCount = skinnedMesh->getJointsCount();
	mat4* finalJointTransformations = new mat4[jointsCount];
	skinnedMesh->getJointsFinalTransformations(&finalJointTransformations);	
	glslProgram->setUniform(jointsTransformUniform, jointsCount, *finalJointTransformations);	
	delete finalJointTransformations;
}