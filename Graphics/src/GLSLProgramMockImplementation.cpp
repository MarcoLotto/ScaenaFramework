/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/
#include <iostream>
#include <fstream>
#include <map>
#include "GLSLProgramMockImplementation.h"
#include "InvalidUniformNameException.h"
#include "InvalidSubroutineNameException.h"
using namespace std;

GLSLProgramMockImplementation::GLSLProgramMockImplementation(){	
}
GLSLProgramMockImplementation::~GLSLProgramMockImplementation(){

}

//Compila el shader pasado por parametro. El segundo argumento es el tipo de shader a compilar
bool GLSLProgramMockImplementation::compileShaderFromFile( const char * fileName, GLSLShader::GLSLShaderType type ){
	return true;
}

//Asocia el indice "location" al nombre de variable del shader
void GLSLProgramMockImplementation::bindAttribLocation( unsigned int location, const char * name){
}

//Asocia el output del fragment shader a un indice "location"
void GLSLProgramMockImplementation::bindFragDataLocation( unsigned int location, const char * name ){

}

//Linkea los shaders al programa
bool GLSLProgramMockImplementation::link(string programBaseName){
	return true;	
}

//Empiza a usar los shaders cargados.
void GLSLProgramMockImplementation::use(){
}

int GLSLProgramMockImplementation::getHandle(){
	return 1;
}


bool GLSLProgramMockImplementation::isLinked(){
	return true;
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, float x, float y, float z){	
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, const glm::vec2 & v){	
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, const glm::vec3 & v){	
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, int size, const glm::vec3 & v){	
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, const glm::vec4 & v){	
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, int size, const glm::vec4 & v){	
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, const glm::mat4 &m){	
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, const glm::mat3 & m){
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, const glm::mat2 & m){
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, int size, const glm::mat4 & m){	
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, int size, const glm::mat3 & m){
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, int size, const glm::mat2 & m){
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, float val ){
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, int val ){
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, unsigned int val ){
}

void GLSLProgramMockImplementation::setUniform(GLSLUniform* uniform, bool val ){
}

void GLSLProgramMockImplementation::setVectorUniform(GLSLUniform* uniform, unsigned int size, float* vector ){
}

void GLSLProgramMockImplementation::setVectorUniform(GLSLUniform* uniform, unsigned int size, int* vector ){
}

void GLSLProgramMockImplementation::setVectorUniform(GLSLUniform* uniform, unsigned int size, unsigned int* vector ){	
}
int GLSLProgramMockImplementation::getUniformLocation(const char *name){
	return 1;
}

void GLSLProgramMockImplementation::changeSubroutineInFragmentShader(const char* subroutineName){
}

void GLSLProgramMockImplementation::initialize(list<string>* inputAttributes, list<string>* outputAttributes, bool usedForFeedback){
}

// Permito setear en clases hijas los nombres stages de shader que quieren usar
void GLSLProgramMockImplementation::setVertexShaderName(string filename){
}
void GLSLProgramMockImplementation::setGeometryShaderName(string filename){
}
void GLSLProgramMockImplementation::setTesselationEvaluationShaderName(string filename){
}
void GLSLProgramMockImplementation::setTesselationControlShaderName(string filename){
}
void GLSLProgramMockImplementation::setComputeShaderName(string filename){
}
void GLSLProgramMockImplementation::setFragmentShaderName(string filename){
}
