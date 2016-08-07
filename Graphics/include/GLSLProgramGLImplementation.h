/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GLSLProgramInterface.h"

class GLSLProgramGLImplementation : public GLSLProgramInterface {

protected:
	float distanceFactor;

private:		
	int programHandle;
	bool linked;
	bool compiled;
	string logString;
	bool firstTime;  //Indica si ya se compilo algun shader
	int numUniforms;

	//Los nombres de los shaders a compilar (de cada tipo)
	string vertexShaderName;
	string geometryShaderName;
	string tesselationEvaluationShaderName;
	string tesselationControlShaderName;
	string computeShaderName;
	string fragmentShaderName;

	// El mapa para cachear uniforms names con locations
	map<string, int> uniformlocationsCache;
	// El mapa para cachear nombres de subrutinas con su location
	map<string, int> subroutinelocationsCache;

	// Consigue la location de un uniform por su nombre. Primero busca en la cache de locations
	int getUniformLocationByName(string name);	
	// Consigue la location de un uniform por su nombre. Primero busca en la cache de locations
	int getSubroutineLocationByName(string name, unsigned int shaderType);
	
public:
	GLSLProgramGLImplementation();
	virtual ~GLSLProgramGLImplementation();

	bool compileShaderFromFile( const char * fileName, GLSLShader::GLSLShaderType type );
	bool link(string programBaseName = "");
	void use();	

	string log();
	int getHandle();
	bool isLinked();
	void bindAttribLocation( unsigned int location, const char * name);
	void bindFragDataLocation( unsigned int location,	const char * name );
	
	void setUniform(GLSLUniform* uniform,float x,float y, float z);
	void setUniform(GLSLUniform* uniform, const glm::vec2 & v);
	void setUniform(GLSLUniform* uniform, const glm::vec3 & v);
	void setUniform(GLSLUniform* uniform, const glm::vec4 & v);
	void setUniform(GLSLUniform* uniform, int size, const glm::vec3 & v);
	void setUniform(GLSLUniform* uniform, int size, const glm::vec4 & v);
	void setUniform(GLSLUniform* uniform, const glm::mat4 & m);
	void setUniform(GLSLUniform* uniform, const glm::mat3 & m);
	void setUniform(GLSLUniform* uniform, const glm::mat2 & m);
	void setUniform(GLSLUniform* uniform, int size, const glm::mat4 & m);
	void setUniform(GLSLUniform* uniform, int size, const glm::mat3 & m);
	void setUniform(GLSLUniform* uniform, int size, const glm::mat2 & m);
	void setUniform(GLSLUniform* uniform, float val );
	void setUniform(GLSLUniform* uniform, int val );
	void setUniform(GLSLUniform* uniform, unsigned int val );
	void setUniform(GLSLUniform* uniform, bool val );
	void setVectorUniform(GLSLUniform* uniform, unsigned int size, float* vector );
	void setVectorUniform(GLSLUniform* uniform, unsigned int size, int* vector );
	void setVectorUniform(GLSLUniform* uniform, unsigned int size, unsigned int* vector );
	int getUniformLocation(const char *name);
	
	// Permito setear en clases hijas los nombres stages de shader que quieren usar
	void setVertexShaderName(string filename);
	void setGeometryShaderName(string filename);
	void setTesselationEvaluationShaderName(string filename);
	void setTesselationControlShaderName(string filename);
	void setComputeShaderName(string filename);
	void setFragmentShaderName(string filename);

	// Cambia la subrutina en el fragment shader
	void changeSubroutineInFragmentShader(const char* subroutineName);

	// Inicializa el shader (compila cada stage de shader y linkea).
	void initialize(list<string>* inputAttributes, list<string>* outputAttributes, bool usedForFeedback=false);	

	//**********************************Implementar en clase hija****************************************	
	//init: Define los buffers de entrada y salida del shader y los shader stages y sus filenames a utilizar
	virtual void init(){};	
	//***************************************************************************************************

	// La distancia hasta el centro del objeto
	float getDistanceFactor(){ return this->distanceFactor;}
	void setDistanceFactor(float distance){ this->distanceFactor = distance; } 
};