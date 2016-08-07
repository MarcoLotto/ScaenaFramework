/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Cubo.h"

class GLSLProgram;
class SistemaIluminacion;

typedef glm::vec3 vec3;
typedef glm::mat4 mat4;

class Marcador{

private:
	vec3 posicion;
	Cubo marcador;
	bool visible;
	static float texturaData[];
	mat4 modelMat;	
	vec3 scale;

public:
	Marcador(float x, float y, float z, vec3 scale);
	void isVisible(bool value);
	void cambiarPosicion(float x, float y, float z);
	void aumentarX(float incremento);
	void aumentarY(float incremento);
	void aumentarZ(float incremento);
	vec3 getPosicion();
	void setPosicion(vec3 posicion);
	void cargar(const char* filename);
	void render(GLSLProgram* shaderProgram, mat4 view, mat4 persp);
	void setMaterial(float Ka, float Kd, float Ks, float brillo);

};

