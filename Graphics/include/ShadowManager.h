/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

#include "Texture.h"
#include "SceneLight.h"
#include "ShadowShader.h"
#include "ObjectManager.h"
#include <map>
using namespace std;

typedef glm::vec2 vec2; 
class RenderPipelineWithGeometryAndBackWrite;
class VarianceShadowMapStage;

class ShadowManager{
		
private:	
	map<SceneLight*, VarianceShadowMapStage*> activeShadowMaps;
	ShadowShader* shadowShader;
	mat4 Bmatrix;  //La bias matrix
	RenderPipelineWithGeometryAndBackWrite* renderPipeline;
	ObjectManager* objectManager;

	VarianceShadowMapStage* generateShadowMap(SceneLight* light, int width, int height);
	void createBiasMatrix();
	float norm(vec3 v);
	float rotationAngleBetweenVectors(vec3 u, vec3 v);	
	
public:
	ShadowManager(ObjectManager* objectManager);
	
	//Creo un nuevo shadow map para esa luz, si ya existe no hago nada
	//Se define tambien el tamaño en pixels del shadowMap
	void createNewShadowMap(SceneLight* light, vec2 shadowMapSize);

	//Consigue el shadow map asociado a esa luz
	Texture* getShadowMap(SceneLight* light);

	//Devuelve la shadowMatrix especifica para esa luz
	mat4 getShadowMatrix(SceneLight* light);

	//Si la luz tiene asociado un shadow map lo libera, sino no hace nada
	void destroyShadowMap(SceneLight* light);

	//Devuelve el numero de shadow maps disponibles en GLSL
	int getMaxShadowMapCount();
	
	//Actualiza el factor del filtro (que tan blur o filtrado esta)
	void updateShadowFilterFactor(SceneLight* light);

	// Actualiza la matriz de vista, desde el punto de vista de la luz
	void updateLightShadowView(SceneLight* light);	
	// Manda a generar la matriz de perspectiva de la sombra
	static void generateShadowPerspective(SceneLight* light);	

	// Se setea el render pipeline para setear las stages de shadow
	void setRenderPipeline(RenderPipelineWithGeometryAndBackWrite* pipeline);

	// Se setea el object manager para poder renderizar las sombras
	void setObjectManager(ObjectManager* objectManager){ this->objectManager = objectManager; }
};