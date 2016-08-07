/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "GraphicArrayContainer.h"
#include "GraphicDevice.h"

#include <string>
using namespace std;

#include "Cubo.h"

#define CANT_VERTICES 24

float Cubo::normalData[] = 
{
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
     
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,

   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,
     
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
     
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,

    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f
};

float Cubo::tangentData[] = 
{
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
     
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,

	0.0f,  -1.0f,  0.0f,
	0.0f,  -1.0f,  0.0f,
	0.0f,  -1.0f,  0.0f,
	0.0f,  -1.0f,  0.0f,
     
    0.0f, 0.0f,  -1.0f,
    0.0f, 0.0f,  -1.0f,
    0.0f, 0.0f,  -1.0f,
    0.0f, 0.0f,  -1.0f,
     
    -1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,

    1.0f,  0.0f, 0.0f,
    1.0f,  0.0f, 0.0f,
    1.0f,  0.0f, 0.0f,
    1.0f,  0.0f, 0.0f
};

float Cubo::positionData[] = 
{
	 0.5f, -0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f
};

float Cubo::texturaDefaultData[] = 
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

Cubo::Cubo(){
	this->factorAlpha = 1.0f;	
	this->normalIntensity = 1.0f;
	this->setMaterial(0.3f, 0.7f, 0.7f, 100.0f);
	this->isTextureDifusa = false;
	this->isTextureNormal = false;
	this->isTextureRefelexion = false;
	this->vao = NULL;
	this->vbo1 = NULL;
	this->vbo2 = NULL;
	this->vbo3 = NULL;
}

Cubo::~Cubo(){
	if(this->vao != NULL)
		delete this->vao;
	if(this->vbo1 != NULL)
		delete this->vbo1;
	if(this->vbo2 != NULL)
		delete this->vbo2;
	if(this->vbo3 != NULL)
		delete this->vbo3;
}

void Cubo::cargarTextureDifusa(const char *pathText, float *textureCoordData){
	this->texturaData = textureCoordData;
	this->texturaDifusa.loadFromFile(pathText);	
	this->isTextureDifusa = true;
}

void Cubo::cargarTextureNormal(const char *pathText){
	this->texturaNormal.loadFromFile(pathText);
	this->isTextureNormal = true;
}

void Cubo::cargarMapaReflexion(const char *pathText){
	this->texturaReflexion.loadFromFile(pathText);
	this->isTextureRefelexion = true;
}

void Cubo::cargar(){	
	this->vao = new VertexArrayObject();
	GraphicDevice* device = GraphicDevice::getInstance();	
	this->vbo1 = new VertexBufferObject(positionData, 3*CANT_VERTICES, 3);
	this->vbo1->load(this->vao);
	this->vbo2 = new VertexBufferObject(normalData, 3*CANT_VERTICES, 3);
	this->vbo2->load(this->vao);
	this->vbo3 = new VertexBufferObject(texturaData, 2*CANT_VERTICES, 2);
	this->vbo3->load(this->vao);	
}

void Cubo::render(GLSLProgram* programaHandler, glm::mat4 viewMat, glm::mat4 modelMat, glm::mat4 perspMat){
	/*
	//Activo el programa de shaders
	programaHandler->use();

	//Cargo matrices de modelo, view, proyeccion y normal
	glm::mat4 VM = viewMat * modelMat;
	programaHandler->setUniform("VMmatrix", VM);
	programaHandler->setUniform("PVMmatrix", perspMat * VM);
		
	glm::mat3 normalMat;
	
	//Armo la matriz normal
	for(int i=0; i < 3; i++){
		for(int j=0; j < 3 ; j++){
			normalMat[i][j] = VM[i][j];
		}
	}
	normalMat = glm::inverse(normalMat);
	normalMat = glm::transpose(normalMat);
	programaHandler->setUniform("normalMatrix", normalMat);

	//Asigno la textura
	if(this->isTextureDifusa)
		texturaDifusa.assignToShader(programaHandler, "diffuseMap", 0);

	//Asigno los materiales (por ahora para todos igual)
	programaHandler->setUniform("mat.Ka", material.Ka);
	programaHandler->setUniform("mat.Kd", material.Kd);
	programaHandler->setUniform("mat.Ks", material.Ks);

	//Mando a renderizar
	GraphicDevice::getInstance()->drawQuads(this->vao->getHandler(), CANT_VERTICES);	
	*/
}


void Cubo::setAlphaReflexion(float alpha){
	this->factorAlpha = alpha;
}

void Cubo::setNormalIntensity(float alpha){
	this->normalIntensity = alpha;
}


void Cubo::setMaterial(float Ka, float Kd, float Ks, float brillo){

	this->material.Ka = Ka;
	this->material.Kd = Kd;
	this->material.Ks = Ks;
	this->material.brillo = brillo;
}

void Cubo::setMaterial(Material mat){

	this->material.Ka = mat.Ka;
	this->material.Kd = mat.Kd;
	this->material.Ks = mat.Ks;
	this->material.brillo = mat.brillo;
}