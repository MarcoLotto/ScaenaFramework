/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GLSLProgram.h"
#include "TransformFeedbackObject.h"

// Interfaz comun para aquellos shaders que dibujan meshes
class MeshDrawerShader : public GLSLProgram{

private:	
	MeshDrawerShader* lowerShader;
	float distanceToChangeShader;
	
protected:
	// Aca los hijos realizan la carga de uniforms y activan el programa de shaders
	virtual void prepareForDraw(Mesh* mesh) = 0;

public:
	MeshDrawerShader() : GLSLProgram(){ this->lowerShader = NULL; this->distanceToChangeShader=99999999.0f; };

	//Dibuja el contenido de un mesh
	virtual void draw(Mesh* mesh);

	//Dibuja el contenido de un mesh utilizando transform feedback
	virtual void draw(Mesh* mesh, TransformFeedbackObject* transformFeedbackObject);

	//Evalua sobre una serie de reglas si es necesario cambiar de shader. Devuelve el shader a utilizar
	virtual MeshDrawerShader* evaluateAndChangeShader(vector<MeshDrawerShader*> *shaderHistory, vec3 centerPoint);

	// Los shaders hijos deben indicar a que distancia se considera que se puede cambiar a un shader de menor calidad (y volver a este)
	void setDistanceToChangeShader(float distance){	this->distanceToChangeShader = distance; }
	float getDistanceToChangeShader(){ return this->distanceToChangeShader; }

	// Los shaders hijos deben indicar a que shader se debe pasar si se tiene que pasar a un shader de menor calidad
	void setLowerShader(MeshDrawerShader* program){ this->lowerShader = program; }
	MeshDrawerShader* getLowerShader(){ return this->lowerShader; }
};
