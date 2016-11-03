/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GLSLUniform.h"
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform2.hpp> 
#include <glm/gtx/projection.hpp> 
#include <string>
#include <list>
using namespace std;

namespace GLSLShader {

	enum GLSLShaderType {
		VERTEX, FRAGMENT, GEOMETRY, TESS_CONTROL,
		TESS_EVALUATION, COMPUTE
	};
};

class GLSLProgramInterface
{	
public:
	virtual bool compileShaderFromFile( const char * fileName, GLSLShader::GLSLShaderType type ) = 0;
	virtual bool link(string programBaseName = "") = 0;
	virtual void use() = 0;	

	virtual int getHandle() = 0;
	virtual bool isLinked() = 0;
	virtual void bindAttribLocation( unsigned int location, const char * name) = 0;
	virtual void bindFragDataLocation( unsigned int location,	const char * name ) = 0;
	
	virtual void setUniform(GLSLUniform* uniform,float x,float y, float z) = 0;
	virtual void setUniform(GLSLUniform* uniform, const glm::vec2 & v) = 0;
	virtual void setUniform(GLSLUniform* uniform, const glm::vec3 & v) = 0;
	virtual void setUniform(GLSLUniform* uniform, const glm::vec4 & v) = 0;
	virtual void setUniform(GLSLUniform* uniform, int size, const glm::vec3 & v) = 0;
	virtual void setUniform(GLSLUniform* uniform, int size, const glm::vec4 & v) = 0;
	virtual void setUniform(GLSLUniform* uniform, const glm::mat4 & m) = 0;
	virtual void setUniform(GLSLUniform* uniform, const glm::mat3 & m) = 0;
	virtual void setUniform(GLSLUniform* uniform, const glm::mat2 & m) = 0;
	virtual void setUniform(GLSLUniform* uniform, int size, const glm::mat4 & m) = 0;
	virtual void setUniform(GLSLUniform* uniform, int size, const glm::mat3 & m) = 0;
	virtual void setUniform(GLSLUniform* uniform, int size, const glm::mat2 & m) = 0;
	virtual void setUniform(GLSLUniform* uniform, float val ) = 0;
	virtual void setUniform(GLSLUniform* uniform, int val ) = 0;
	virtual void setUniform(GLSLUniform* uniform, unsigned int val ) = 0;
	virtual void setUniform(GLSLUniform* uniform, bool val ) = 0;
	virtual void setVectorUniform(GLSLUniform* uniform, unsigned int size, float* vector ) = 0;
	virtual void setVectorUniform(GLSLUniform* uniform, unsigned int size, int* vector ) = 0;
	virtual void setVectorUniform(GLSLUniform* uniform, unsigned int size, unsigned int* vector ) = 0;
	virtual int getUniformLocation(const char *name) = 0;

	// Permito setear en clases hijas los nombres stages de shader que quieren usar
	virtual void setVertexShaderName(string filename) = 0;	
	virtual void setGeometryShaderName(string filename) = 0;	
	virtual void setTesselationEvaluationShaderName(string filename) = 0;	
	virtual void setTesselationControlShaderName(string filename) = 0;	
	virtual void setComputeShaderName(string filename) = 0;	
	virtual void setFragmentShaderName(string filename) = 0;	
	
	// Cambia la subrutina en el fragment shader
	virtual void changeSubroutineInFragmentShader(const char* subroutineName) = 0;

	// Inicializa el shader (compila cada stage de shader y linkea).
	virtual void initialize(list<string>* inputAttributes, list<string>* outputAttributes, bool usedForFeedback=false) = 0;	

	//**********************************Implementar en clase hija****************************************	
	//init: Define los buffers de entrada y salida del shader y los shader stages y sus filenames a utilizar
	virtual void init(){};	
	//***************************************************************************************************

	// La distancia hasta el centro del objeto
	virtual float getDistanceFactor() = 0;
	virtual void setDistanceFactor(float distance) = 0;
};