/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <map>
#include "GLSLProgramGLImplementation.h"
#include "InvalidUniformNameException.h"
#include "InvalidSubroutineNameException.h"
#include "Logger.h"
using namespace std;

GLSLProgramGLImplementation::GLSLProgramGLImplementation(){
	this->programHandle = 0;	
	this->linked = false;
	this->compiled = false;
	this->logString = "";
	firstTime=true;

	this->vertexShaderName = "";
	this->geometryShaderName = "";
	this->tesselationEvaluationShaderName = "";
	this->tesselationControlShaderName = "";
	this->computeShaderName = "";
	this->fragmentShaderName = "";	
}
GLSLProgramGLImplementation::~GLSLProgramGLImplementation(){

}

int GLSLProgramGLImplementation::getUniformLocationByName(string name){
	// Verifico si existe en la cache, si existe devuelvo la location
	map<string, int>::iterator it = this->uniformlocationsCache.find(name);
	if(it != this->uniformlocationsCache.end()){
		return it->second;
	}

	// No existe en cache, si existe en el shader agrego la location a al cache
	int location = glGetUniformLocation(this->getHandle(), name.c_str());
	if(location != -1){
		this->uniformlocationsCache[name] = location; 
		return location;
	}
	// La location no existe en el shader, no deberia estar buscandola
	// throw new InvalidUniformNameException(name);	  // REVIEW: HABILITAR ESTO!!!!!!!!!!!!!!!!!!!!11
}

int GLSLProgramGLImplementation::getSubroutineLocationByName(string name, unsigned int shaderType){
	// Verifico si existe en la cache, si existe devuelvo la location
	map<string, int>::iterator it = this->subroutinelocationsCache.find(name);
	if(it != this->subroutinelocationsCache.end()){
		return it->second;
	}

	// No existe en cache, si existe en el shader agrego la location a al cache
	int location = glGetSubroutineIndex( this->getHandle(), GL_FRAGMENT_SHADER, name.c_str());
	if(location != -1){
		this->subroutinelocationsCache[name] = location; 
		return location;
	}
	// La location no existe en el shader, no deberia estar buscandola
	throw new InvalidSubroutineNameException(name);	
}

//Compila el shader pasado por parametro. El segundo argumento es el tipo de shader a compilar
bool GLSLProgramGLImplementation::compileShaderFromFile( const char * fileName, GLSLShader::GLSLShaderType type ){

	//Creo el shader correspondiente
	GLuint shader = 0;
	if(type == GLSLShader::VERTEX)
		shader = glCreateShader( GL_VERTEX_SHADER );
	else if(type == GLSLShader::FRAGMENT)
		shader = glCreateShader( GL_FRAGMENT_SHADER );
	else if(type == GLSLShader::GEOMETRY)
		shader = glCreateShader( GL_GEOMETRY_SHADER );
	else if(type == GLSLShader::TESS_CONTROL)
		shader = glCreateShader( GL_TESS_CONTROL_SHADER );
	else if(type == GLSLShader::TESS_EVALUATION)
		shader = glCreateShader( GL_TESS_EVALUATION_SHADER );
	else if(type == GLSLShader::COMPUTE)
		shader = glCreateShader( GL_COMPUTE_SHADER );
	if( 0 == shader )
	{
		logString =  "Error creando el shader " + string(fileName);
		Logger::getInstance()->logError(new Log(logString));
		this->compiled = false;
		return false;
	}

	//Cargo el codigo fuente
	std::ifstream v_shader_file(fileName, std::ifstream::in);
    std::string v_str((std::istreambuf_iterator<char>(v_shader_file)), std::istreambuf_iterator<char>());
    const char* vs_code_array[] = {v_str.c_str()};        
    glShaderSource( shader, 1, vs_code_array, NULL);

	//Compilo el shader
	glCompileShader( shader );

	//Verifico que se haya compilado correctamente
	GLint result;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &result );
	if( GL_FALSE == result )
	{
		logString =  "Error compilando el shader " + string(fileName);
		Logger::getInstance()->logError(new Log(logString));

		GLint logLen;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLen );
		if( logLen > 0 )
		{
			char * log = (char *)malloc(logLen);
		
		GLsizei written;
		glGetShaderInfoLog(shader, logLen, &written, log);

		logString =  "Shader log: \n" + string(log);
		Logger::getInstance()->logError(new Log(logString));

		free(log);		
		}
		this->compiled = false;
		return false;
	}

	//Creo el program handle
	if(firstTime){
		this->programHandle = glCreateProgram();
		if( 0 == programHandle )
		{
			this->compiled=false;
			logString =  "Error creando el shader program handler " + string(fileName);
			Logger::getInstance()->logError(new Log(logString));		
		}
		firstTime = false;
	}
		
	//Asocio el shader al program handle
	glAttachShader( programHandle, shader );
	glDeleteShader(shader);

	this->compiled = true;
	return true;
}

//Asocia el indice "location" al nombre de variable del shader
void GLSLProgramGLImplementation::bindAttribLocation( GLuint location, const char * name){
	glBindAttribLocation( this->programHandle, location, name );
}

//Asocia el output del fragment shader a un indice "location"
void GLSLProgramGLImplementation::bindFragDataLocation( GLuint location, const char * name ){
	glBindFragDataLocation(this->programHandle, location, name);
}

//Linkea los shaders al programa
bool GLSLProgramGLImplementation::link(string programBaseName){
	
	//linkeo el shader
	glLinkProgram( programHandle );

	GLint status;
	glGetProgramiv( programHandle, GL_LINK_STATUS, &status );
	if( GL_FALSE == status ) {
		logString =  "Error linkeando el shader " + string(programBaseName);
		Logger::getInstance()->logError(new Log(logString));

		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH,
		&logLen);
		if( logLen > 0 )
		{
			char * log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen,
			&written, log);
	
			logString =  "Shader log: \n" + string(log);
			Logger::getInstance()->logError(new Log(logString));

			free(log);
		}
		this->linked = false;
		return false;
	}	

	this->linked = true;
	return true;	
}

//Empiza a usar los shaders cargados.
void GLSLProgramGLImplementation::use(){
	if(this->linked)
		glUseProgram( programHandle );
}

int GLSLProgramGLImplementation::getHandle(){
	return this->programHandle;
}


bool GLSLProgramGLImplementation::isLinked(){
	return this->linked;
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, float x, float y, float z){	
	glUniform3f(uniform->getUniformLocation(), x, y, z);
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, const glm::vec2 & v){	
	glUniform2fv(uniform->getUniformLocation(), 1, &v[0]);
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, const glm::vec3 & v){	
	glUniform3fv(uniform->getUniformLocation(), 1, &v[0]);
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, int size, const glm::vec3 & v){	
	glUniform3fv(uniform->getUniformLocation(), size, &v[0]);
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, const glm::vec4 & v){	
	glUniform4fv(uniform->getUniformLocation(), 1, &v[0]);
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, int size, const glm::vec4 & v){	
	glUniform4fv(uniform->getUniformLocation(), size, &v[0]);
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, const glm::mat4 &m){	
	glUniformMatrix4fv(uniform->getUniformLocation(), 1, GL_FALSE, &m[0][0]);
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, const glm::mat3 & m){
	glUniformMatrix3fv(uniform->getUniformLocation(), 1, GL_FALSE, &m[0][0]);
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, const glm::mat2 & m){
	glUniformMatrix2fv(uniform->getUniformLocation(), 1, GL_FALSE, &m[0][0]);
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, int size, const glm::mat4 & m){	
	glUniformMatrix4fv(uniform->getUniformLocation(), size, GL_FALSE, &m[0][0]);
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, int size, const glm::mat3 & m){
	glUniformMatrix3fv(uniform->getUniformLocation(), size, GL_FALSE, &m[0][0]);
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, int size, const glm::mat2 & m){
	glUniformMatrix2fv(uniform->getUniformLocation(), size, GL_FALSE, &m[0][0]);
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, float val ){
	glUniform1f(uniform->getUniformLocation(), val);
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, int val ){
	glUniform1i(uniform->getUniformLocation(), val);
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, unsigned int val ){
	glUniform1ui(uniform->getUniformLocation(), val);
}

void GLSLProgramGLImplementation::setUniform(GLSLUniform* uniform, bool val ){
	glUniform1i(uniform->getUniformLocation(), val);
}

void GLSLProgramGLImplementation::setVectorUniform(GLSLUniform* uniform, unsigned int size, float* vector ){
	glUniform1fv(uniform->getUniformLocation(), size, vector);
}

void GLSLProgramGLImplementation::setVectorUniform(GLSLUniform* uniform, unsigned int size, int* vector ){
	glUniform1iv(uniform->getUniformLocation(), size, vector);
}

void GLSLProgramGLImplementation::setVectorUniform(GLSLUniform* uniform, unsigned int size, unsigned int* vector ){	
	glUniform1uiv(uniform->getUniformLocation(), size, vector);
}
int GLSLProgramGLImplementation::getUniformLocation(const char *name){
	return getUniformLocationByName(name);
}

void GLSLProgramGLImplementation::changeSubroutineInFragmentShader(const char* subroutineName){
	unsigned int index = this->getSubroutineLocationByName(subroutineName, GL_FRAGMENT_SHADER);
	glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &index);
}

void GLSLProgramGLImplementation::initialize(list<string>* inputAttributes, list<string>* outputAttributes, bool usedForFeedback){
	// Evito que un shader se inicialize dos veces
	if(!this->firstTime){
		return;
	}
	
	// Linux
	//__glewBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC) glXGetProcAddress( "glBindFragDataLocation" );
	// Windows
	//__glewBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC) wglGetProcAddress( "glBindFragDataLocation" );
	
	//Compilo los shaders (dependiendo de cuales haya disponibles)
	if(this->vertexShaderName.compare("") != 0)
		this->compileShaderFromFile(this->vertexShaderName.c_str(), GLSLShader::VERTEX);
	if(this->geometryShaderName.compare("") != 0)
		this->compileShaderFromFile(this->geometryShaderName.c_str(), GLSLShader::GEOMETRY);
	if(this->tesselationEvaluationShaderName.compare("") != 0)
		this->compileShaderFromFile(this->tesselationEvaluationShaderName.c_str(), GLSLShader::TESS_EVALUATION);
	if(this->tesselationControlShaderName.compare("") != 0)
		this->compileShaderFromFile(this->tesselationControlShaderName.c_str(), GLSLShader::TESS_CONTROL);
	if(this->fragmentShaderName.compare("") != 0)
		this->compileShaderFromFile(this->fragmentShaderName.c_str(), GLSLShader::FRAGMENT);	

	//Mapeo los atributos de entrada
	unsigned int i = 0;
	std::list<string>::iterator inputIt = inputAttributes->begin();
	while(inputIt != inputAttributes->end()){
		glBindAttribLocation( this->getHandle() , i, (*inputIt).c_str() );
		++inputIt;
		++i;
	}
	//Mapeo los atributos de salida (dependiendo si los uso para feedback o no)
	if(!usedForFeedback){
		i = 0;
		std::list<string>::iterator outputIt = outputAttributes->begin();
		while(outputIt != outputAttributes->end()){
			glBindFragDataLocation(this->getHandle(), i, (*outputIt).c_str());
			++outputIt;
			++i;
		}
	}
	else{
		unsigned int i = 0;
		const char** output = new const char*[outputAttributes->size()]();
		std::list<string>::iterator outputIt = outputAttributes->begin();
		while(outputIt != outputAttributes->end()){				
			output[i] = (*outputIt).c_str();
			++outputIt;
			++i;
		}
		glTransformFeedbackVaryings(this->getHandle(), outputAttributes->size(), output, GL_SEPARATE_ATTRIBS);
		delete output;		
	}
	//Linkeo el programa (le paso el nombre del vertex shader para el logguer nomas)
	this->link(string(vertexShaderName).substr(0, this->vertexShaderName.length() - 5));	
}


// Permito setear en clases hijas los nombres stages de shader que quieren usar
void GLSLProgramGLImplementation::setVertexShaderName(string filename){
	this->vertexShaderName = filename;
}
void GLSLProgramGLImplementation::setGeometryShaderName(string filename){
	this->geometryShaderName = filename;
}
void GLSLProgramGLImplementation::setTesselationEvaluationShaderName(string filename){
	this->tesselationEvaluationShaderName = filename;
}
void GLSLProgramGLImplementation::setTesselationControlShaderName(string filename){
	this->tesselationControlShaderName = filename;
}
void GLSLProgramGLImplementation::setComputeShaderName(string filename){
	this->computeShaderName = filename;
}
void GLSLProgramGLImplementation::setFragmentShaderName(string filename){
	this->fragmentShaderName = filename;
}
