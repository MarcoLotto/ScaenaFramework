/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <list>
#include "Mesh.h"
using namespace std;

class SkinnedDrawerHelper{

public:
	// Inicializa los atributos de entrada del shader para skinned mesh
	static void skinAttributesInit(GLSLProgram* glslProgram, list<string>* inputAttributes);

	// Inicializa los uniforms para skinned mesh
	static void skinUniformsInit(GLSLProgram* glslProgram, GLSLUniform** jointsTransformUniform);

	// Agrega los uniforms para procesar el skeleton en shader
	static void skinDraw(GLSLProgram* glslProgram, Mesh* mesh, GLSLUniform* jointsTransformUniform);
};