/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "FileTexture.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

#define PI 3.1415f

#include <string>
using namespace std;

#include <vector>

class Texture;
class GLSLProgram;
class SistemaIluminacion;

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;


class Toroide{
	
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

	//static float positionData[];
	//float* texturaData;
	static float texturaDefaultData[];
	bool segundaTexture;

	 //glm::mat4 view_matrix;

    float* sphere_vertex_buffer;
    unsigned int* sphere_index_buffer;
	float* normalData;
	float* texturaData;
	float* tangentData;

    std::vector<float> vertex_buffer;
    std::vector<float> normal_buffer;
	std::vector<float> texture_buffer;
	std::vector<float> tangent_buffer;
	std::vector<unsigned int> index_buffer;
    

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
	Toroide();
	virtual ~Toroide();

	//setAlpha: determina el valor alpha de la segunda textura. Este valor tiene que estar entre 0.0f y 1.0f.
	//Si es 0.0f, el shader directamente no la procesa.
	void setAlphaReflexion(float alpha);
	void setNormalIntensity(float alpha);
		
	//setMaterial: Determina las propiedades del cubo. Ka, Kd, Ks: Atenuacion sobre el color ambiente, difuso y specular respectivamente.
	//Brillo: es el brillo specular.
	void setMaterial(float Ka, float Kd, float Ks, float brillo);
	void setMaterial(Material mat);

	//Carga el esfera
	void cargar();

	//Renderiza el cubo
	void render(GLSLProgram* programaHandler, glm::mat4 viewMat, glm::mat4 modelMat, glm::mat4 perspMat);

	//Carga cada una de las texturas
	void cargarTextureDifusa(const char *pathText, float *textureCoordData);
	void cargarTextureNormal(const char *pathText);
	void cargarMapaReflexion(const char *pathText);

};
