/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GLSLProgram.h"
#include "LightingManager.h"
#include "Texture.h"

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;

class PhongLightingWithSSAOShader : public GLSLProgram {

private:
	Mesh* rectangleMesh;
	static PhongLightingWithSSAOShader* instance;
	
	//Textures a utilizar
	Texture* texPosition;
	Texture* texNormal;
	Texture* texDiffuse;
	Texture* texMaterial;
	Texture* ssaoData;

	PhongLightingWithSSAOShader();

	GLSLUniform* modelMatrixUniform;
	GLSLUniform* proyMatrixUniform;
	GLSLUniform* sceneProjUniform;
	GLSLUniform* depthTexUniform;
	GLSLUniform* normalTexUniform;
	GLSLUniform* colorTexUniform;
	GLSLUniform* materialTexUniform;
	GLSLUniform* ssaoTextureUniform;
	GLSLUniform* gammaUniform;

	LightUniformsContainer* lightUniformsContainer;

public:	
	static PhongLightingWithSSAOShader* getInstance();

	// Prepara el ambiente para el renderizado. Debe llamarse antes de render si es que cambio algun parametro.
	void prepareForDraw(Texture* position, Texture* normal, Texture* diffuse, Texture* material, Texture* ssaoData);

	// Renderiza el cuadrado de renderizado
	// La matriz de vista y proyeccion de escena provista no moodifica al rectangulo, sino a la escena a la que este representa
	void draw(LightingManager *sisIlum, glm::mat4 sceneView, glm::mat4 sceneProy, glm::mat4 modelMat, glm::mat4 proyMat);
	
	// Inicializa los shaders
	void init();
};