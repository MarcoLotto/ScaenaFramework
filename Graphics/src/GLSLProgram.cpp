/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "Mesh.h"
#include "GLSLProgram.h"
#include "GLSLProgramGLImplementation.h"
#include "GLSLProgramGLES3Implementation.h"
#include "GLSLProgramGLES2Implementation.h"
#include "ApiNotBindException.h"
#include "ApiVariableTypeDoesNotExistException.h"

int GLSLProgram::apiInUse = -1;  // Indicador del API que se utiliza

// Para elegir que api utilar
void GLSLProgram::bindApi(int apiId){
	apiInUse = apiId; 
}

GLSLProgramInterface* GLSLProgram::createProgramImplementation(){
	if(apiInUse == -1){
		throw new ApiNotBindException("No se indico ningun API a usar al GLSLProgram");
	}
	// Se deben definir en el preprocesador las opciones disponibles para la plataforma en donde se compila
	switch(apiInUse){
		#ifdef OPENGL4
		case OPENGL_API:
			return new GLSLProgramGLImplementation(); 
		break;
		#endif
		#ifdef OPENGLES3 
		case OPENGLES_3_API:
			return new GLSLProgramGLES3Implementation(); 
		break;
		#endif
		#ifdef OPENGLES2
		case OPENGLES_2_API:
			return new GLSLProgramGLES2Implementation(); 
		break;
		#endif
		default:
			throw new ApiVariableTypeDoesNotExistException("El API informada al GLSLProgram no es correcta");
		break;
	}
}

GLSLProgram::GLSLProgram(){
	this->implementation = this->createProgramImplementation();	
}

GLSLProgram::~GLSLProgram(){
	delete this->implementation;	
}

//Compila el shader pasado por parametro. El segundo argumento es el tipo de shader a compilar
bool GLSLProgram::compileShaderFromFile( const char * fileName, GLSLShader::GLSLShaderType type ){
	return this->implementation->compileShaderFromFile(fileName, type);
}

//Asocia el indice "location" al nombre de variable del shader
void GLSLProgram::bindAttribLocation( unsigned int location, const char * name){
	this->implementation->bindAttribLocation(location, name);
}

//Asocia el output del fragment shader a un indice "location"
void GLSLProgram::bindFragDataLocation( unsigned int location, const char * name ){
	this->implementation->bindFragDataLocation(location, name);
}

//Linkea los shaders al programa
bool GLSLProgram::link(string programBaseName){
	return this->implementation->link(programBaseName);	
}

//Empiza a usar los shaders cargados.
void GLSLProgram::use(){
	this->implementation->use();
}

int GLSLProgram::getHandle(){
	return this->implementation->getHandle();
}

bool GLSLProgram::isLinked(){
	return this->implementation->isLinked();
}

void GLSLProgram::setUniform(GLSLUniform* uniform, float x, float y, float z){
	this->implementation->setUniform(uniform, x, y, z);
}

void GLSLProgram::setUniform(GLSLUniform* uniform, const glm::vec2 & v){
	this->implementation->setUniform(uniform, v);
}

void GLSLProgram::setUniform(GLSLUniform* uniform, const glm::vec3 & v){
	this->implementation->setUniform(uniform, v);
}

void GLSLProgram::setUniform(GLSLUniform* uniform, int size, const glm::vec3 & v){
	this->implementation->setUniform(uniform, v);
}

void GLSLProgram::setUniform(GLSLUniform* uniform, const glm::vec4 & v){
	this->implementation->setUniform(uniform, v);
}

void GLSLProgram::setUniform(GLSLUniform* uniform, int size, const glm::vec4 & v){
	this->implementation->setUniform(uniform, v);
}

void GLSLProgram::setUniform(GLSLUniform* uniform, const glm::mat4 &m){
	this->implementation->setUniform(uniform, m);
}

void GLSLProgram::setUniform(GLSLUniform* uniform, const glm::mat3 & m){
	this->implementation->setUniform(uniform, m);
}

void GLSLProgram::setUniform(GLSLUniform* uniform, const glm::mat2 & m){
	this->implementation->setUniform(uniform, m);
}

void GLSLProgram::setUniform(GLSLUniform* uniform, int size, const glm::mat4 &m){
	this->implementation->setUniform(uniform, size, m);
}

void GLSLProgram::setUniform(GLSLUniform* uniform, int size, const glm::mat3 & m){
	this->implementation->setUniform(uniform, size, m);
}

void GLSLProgram::setUniform(GLSLUniform* uniform, int size, const glm::mat2 & m){
	this->implementation->setUniform(uniform, size, m);
}


void GLSLProgram::setUniform(GLSLUniform* uniform, float val ){
	this->implementation->setUniform(uniform, val);
}

void GLSLProgram::setUniform(GLSLUniform* uniform, int val ){
	this->implementation->setUniform(uniform, val);
}

void GLSLProgram::setUniform(GLSLUniform* uniform, unsigned int val ){
	this->implementation->setUniform(uniform, val);
}


void GLSLProgram::setUniform(GLSLUniform* uniform, bool val ){
	this->implementation->setUniform(uniform, val);
}

void GLSLProgram::setVectorUniform(GLSLUniform* uniform, unsigned int size, float* vector ){
	this->implementation->setVectorUniform(uniform, size, vector);
}

void GLSLProgram::setVectorUniform(GLSLUniform* uniform, unsigned int size, int* vector ){
	this->implementation->setVectorUniform(uniform, size, vector);
}

void GLSLProgram::setVectorUniform(GLSLUniform* uniform, unsigned int size, unsigned int* vector ){
	this->implementation->setVectorUniform(uniform, size, vector);
}

int GLSLProgram::getUniformLocation(const char *name){
	return this->implementation->getUniformLocation(name);
}

void GLSLProgram::changeSubroutineInFragmentShader(const char* subroutineName){
	this->implementation->changeSubroutineInFragmentShader(subroutineName);
}

void GLSLProgram::initialize(list<string>* inputAttributes, list<string>* outputAttributes, bool usedForFeedback){
	this->implementation->initialize(inputAttributes, outputAttributes, usedForFeedback);
}

// Permito setear en clases hijas los nombres stages de shader que quieren usar
void GLSLProgram::setVertexShaderName(string filename){
	this->implementation->setVertexShaderName(filename);
}
void GLSLProgram::setGeometryShaderName(string filename){
	this->implementation->setGeometryShaderName(filename);
}
void GLSLProgram::setTesselationEvaluationShaderName(string filename){
	this->implementation->setTesselationEvaluationShaderName(filename);
}
void GLSLProgram::setTesselationControlShaderName(string filename){
	this->implementation->setTesselationControlShaderName(filename);
}
void GLSLProgram::setComputeShaderName(string filename){
	this->implementation->setComputeShaderName(filename);
}
void GLSLProgram::setFragmentShaderName(string filename){
	this->implementation->setFragmentShaderName(filename);
}

// La distancia hasta el centro del objeto
float GLSLProgram::getDistanceFactor(){ return this->implementation->getDistanceFactor(); }
void GLSLProgram::setDistanceFactor(float distance){ this->implementation->setDistanceFactor(distance); } 
