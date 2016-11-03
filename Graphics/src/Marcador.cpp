/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include <glm/glm.hpp> 

#include "Texture.h"
#include "GLSLProgram.h"
#include "Cubo.h"
#include "Marcador.h"

float Marcador::texturaData[] = 
{
    0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f
};

Marcador::Marcador(float x, float y, float z, vec3 scale){
	this->marcador=Cubo();
	this->scale = scale;
	this->posicion=vec3(x, y, z);	
	this->visible=true;
	this->modelMat = glm::scale(glm::mat4(1.0f), this->scale);
}

void Marcador::isVisible(bool value){
	this->visible=value;
}
void Marcador::cambiarPosicion(float x, float y, float z){
	this->posicion.x=x;
	this->posicion.y=y;
	this->posicion.z=z;	
	this->modelMat = glm::translate(mat4(1.0f), this->posicion);
	this->modelMat = glm::scale(this->modelMat, this->scale);
}

void Marcador::setPosicion(vec3 posicion){
	this->cambiarPosicion(posicion.x, posicion.y, posicion.z);
}
void Marcador::aumentarX(float incremento){
	this->posicion.x+=incremento;
	this->modelMat = glm::translate(mat4(1.0f), this->posicion);
	this->modelMat = glm::scale(this->modelMat, this->scale);
}
void Marcador::aumentarY(float incremento){
	this->posicion.y += incremento;
	this->modelMat = glm::translate(mat4(1.0f), this->posicion);
	this->modelMat = glm::scale(this->modelMat,this->scale);
}
void Marcador::aumentarZ(float incremento){
	this->posicion.z += incremento;
	this->modelMat = glm::translate(mat4(1.0f), this->posicion);
	this->modelMat = glm::scale(this->modelMat, this->scale);
}
vec3 Marcador::getPosicion(){
	return this->posicion;
}
void Marcador::cargar(const char* filename){
	marcador.cargarTextureDifusa(filename, this->texturaData);
	marcador.cargar();
}

void Marcador::render(GLSLProgram* shaderProgram, glm::mat4 view, glm::mat4 persp){
	marcador.render(shaderProgram, view, this->modelMat, persp);
}

void Marcador::setMaterial(float Ka, float Kd, float Ks, float brillo){
	this->marcador.setMaterial(Ka, Ks, Ks, brillo);
}

