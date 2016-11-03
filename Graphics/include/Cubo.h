/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <glm/glm.hpp> 

#include "FileTexture.h"
#include "GLSLProgram.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;

#define PI 3.14159265f



class Cubo{

struct Material{
	float Ka;
	float Kd;
	float Ks;
	float brillo;
};


private:
	VertexBufferObject* vbo1;
	VertexBufferObject* vbo2;
	VertexBufferObject* vbo3;
	VertexArrayObject* vao;

	static float positionData[];
	static float normalData[];
	static float tangentData[];
	float* texturaData;
	static float texturaDefaultData[];
	bool segundaTexture;


	FileTexture texturaDifusa;
	FileTexture texturaNormal;
	FileTexture texturaReflexion;
	bool isTextureDifusa;
	bool isTextureNormal;
	bool isTextureRefelexion;
	float factorAlpha;
	float normalIntensity;
	Material material;
	
	
public:	
	//Constructor
	Cubo();
	virtual ~Cubo();

	//setAlpha: determina el valor alpha de la segunda textura. Este valor tiene que estar entre 0.0f y 1.0f.
	//Si es 0.0f, el shader directamente no la procesa.
	void setAlphaReflexion(float alpha);
	void setNormalIntensity(float alpha);
		
	//setMaterial: Determina las propiedades del cubo. Ka, Kd, Ks: Atenuacion sobre el color ambiente, difuso y specular respectivamente.
	//Brillo: es el brillo specular.
	void setMaterial(float Ka, float Kd, float Ks, float brillo);
	void setMaterial(Material mat);

	//Carga el cubo
	void cargar();

	//Renderiza el cubo
	void render(GLSLProgram* programaHandler, glm::mat4 viewMat, glm::mat4 modelMat, glm::mat4 perspMat);

	//Carga cada una de las texturas
	void cargarTextureDifusa(const char *pathText, float *textureCoordData);
	void cargarTextureNormal(const char *pathText);
	void cargarMapaReflexion(const char *pathText);

};
