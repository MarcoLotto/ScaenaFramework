/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "GraphicArrayContainer.h"
#include "GraphicDevice.h"

#include <glm/glm.hpp> 

#include "Texture.h"
#include "GLSLProgram.h"

#include "Cilindro.h"

#define TILE_FACTOR 4
#define CANT_VERTICES 24

Cilindro::Cilindro(){
	this->sphere_vertex_buffer = NULL;
    this->sphere_index_buffer = NULL;
	factorAlpha = 1.0f;	
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

Cilindro::~Cilindro(){
	if(this->vao != NULL)
		delete this->vao;
	if(this->vbo1 != NULL)
		delete this->vbo1;
	if(this->vbo2 != NULL)
		delete this->vbo2;
	if(this->vbo3 != NULL)
		delete this->vbo3;
}

void Cilindro::cargarTextureDifusa(const char *pathText, float *textureCoordData){
	//this->texturaData = textureCoordData;
	this->texturaDifusa.loadFromFile(pathText);	
	this->isTextureDifusa = true;
}

void Cilindro::cargarTextureNormal(const char *pathText){
	this->texturaNormal.loadFromFile(pathText);
	this->isTextureNormal = true;
}

void Cilindro::cargarMapaReflexion(const char *pathText){
	this->texturaReflexion.loadFromFile(pathText);
	this->isTextureRefelexion = true;
}

void Cilindro::cargar(){

	const float radio = 1.0;
	const unsigned int nlatitud = 32;
	const unsigned int nlongitud = 32;
	const unsigned int cantDivisionesCuerpo = 6;
    //float PI = 3.1415f;
	
	if (this->vertex_buffer.size() >0)
    {
        this->vertex_buffer.clear();
    }
	
	for (unsigned int i = 0; i <= nlatitud ; i++)
	{
		float theta = i * PI / nlatitud;
		float senTheta = sin(theta);
		float cosTheta = cos(theta);
	
		for (unsigned int j = 0 ; j <= nlongitud; j++)
		{
			float phi = j * 2 * PI/ nlongitud;
			float senPhi = sin(phi);
			float cosPhi = cos(phi);

			float x = cosPhi * senTheta;
			float y = 0;
			float z = senPhi * senTheta;
			float u = 1 - (float)(j/(float)nlongitud);
			float v = 1 - (float)(i/(float)nlatitud);

			if((i > nlatitud/2)&&( i <= nlatitud/2 + cantDivisionesCuerpo)){
				x = cosPhi;
				y=( i - (float)nlatitud/2) / float(cantDivisionesCuerpo);
				z = senPhi;
				this->normal_buffer.push_back(x); 
				this->normal_buffer.push_back(0); 
				this->normal_buffer.push_back(z); 
				
			}
			else if((i < nlatitud/2)&&( i >= nlatitud/2 - cantDivisionesCuerpo)){
				x = cosPhi;
				y= ( i - (float)nlatitud/2) / float(cantDivisionesCuerpo);
				z = senPhi;	
				this->normal_buffer.push_back(x); 
				this->normal_buffer.push_back(0); 
				this->normal_buffer.push_back(z); 
			}
			else if( i > nlatitud/2 + cantDivisionesCuerpo){
				y = 1.0f;
				this->normal_buffer.push_back(0); 
				this->normal_buffer.push_back(y); 
				this->normal_buffer.push_back(0); 
			}
			else if( i < nlatitud/2 - cantDivisionesCuerpo){
				y = -1.0f;
				this->normal_buffer.push_back(0); 
				this->normal_buffer.push_back(y); 
				this->normal_buffer.push_back(0); 
			}
			else{
				this->normal_buffer.push_back(x); 
				this->normal_buffer.push_back(0); 
				this->normal_buffer.push_back(z); 
			}
			
			glm::vec3 vectorAux = glm::cross(vec3(0.0f, 0.0f, 1.0f), vec3(x,y,z));
			 
			this->tangent_buffer.push_back(vectorAux.x);
			 this->tangent_buffer.push_back(vectorAux.y);
			 this->tangent_buffer.push_back(vectorAux.z);
	
			
			this->texture_buffer.push_back(u * TILE_FACTOR); 
			this->texture_buffer.push_back(v * TILE_FACTOR); 
	
			this->vertex_buffer.push_back(radio * x); 
			this->vertex_buffer.push_back(radio * y); 
			this->vertex_buffer.push_back(radio * z); 

		}

	}

    //PosicionData
    if (this->sphere_vertex_buffer != NULL)
    {
        delete[] this->sphere_vertex_buffer;
    }
    this->sphere_vertex_buffer = new float[this->vertex_buffer.size()];


    std::vector<float>::iterator vertex_it;
    unsigned int v = 0;
    for (vertex_it = this->vertex_buffer.begin(); vertex_it != this->vertex_buffer.end(); vertex_it++)
    {
        this->sphere_vertex_buffer[v] = *vertex_it;
        v++;
    }

	//Normal Data
	if (this->normalData != NULL)
    {
        delete[] this->normalData;
    }
	this->normalData = new float[this->normal_buffer.size()];

	v = 0;
    for (vertex_it = this->normal_buffer.begin(); vertex_it != this->normal_buffer.end(); vertex_it++)
    {
        this->normalData[v] = *vertex_it;
        v++;
    }

		//	Tangent Data
	if (this->tangentData != NULL)
    {
        delete[] this->tangentData;
    }
	this->tangentData = new float[this->tangent_buffer.size()];

	v = 0;
    for (vertex_it = this->tangent_buffer.begin(); vertex_it != this->tangent_buffer.end(); vertex_it++)
    {
        this->tangentData[v] = *vertex_it;
        v++;
    }
	//Texture Data

	if (this->texturaData != NULL)
    {
        delete[] this->texturaData;
    }
    this->texturaData = new float[this->texture_buffer.size()];

	v = 0;
    for (vertex_it = this->texture_buffer.begin(); vertex_it != this->texture_buffer.end(); vertex_it++)
    {

        this->texturaData[v] = *vertex_it;
        v++;
    }

	for(float i = 0; i< nlatitud; i++){
		for(float j = 0; j < nlongitud; j++){
			int first = (i * (nlongitud + 1)) + j;	
			int second = first + nlongitud + 1;
			index_buffer.push_back(first);
			index_buffer.push_back(second);
			index_buffer.push_back(first + 1);
		
			index_buffer.push_back(second);	
			index_buffer.push_back(second + 1);
			index_buffer.push_back(first + 1);
		}	

	}

	//Index Data

	if (this->sphere_index_buffer != NULL)
    {
        delete[] this->sphere_index_buffer;
    }
    this->sphere_index_buffer = new unsigned int[this->index_buffer.size()];
	std::vector<unsigned int>::iterator index_it;
	v = 0;
    for (index_it = this->index_buffer.begin(); index_it != this->index_buffer.end(); index_it++)
    {
        this->sphere_index_buffer[v] = *index_it;
        v++;
    }

	// Cargo la geometria en memoria de video
	this->vao = new VertexArrayObject();
	GraphicDevice* device = GraphicDevice::getInstance();	
	this->vbo1 = new VertexBufferObject(sphere_vertex_buffer, vertex_buffer.size(), 3);
	this->vbo1->load(this->vao);
	this->vbo2 = new VertexBufferObject(normalData, normal_buffer.size(), 3);
	this->vbo2->load(this->vao);
	this->vbo3 = new VertexBufferObject(texturaData, texture_buffer.size(), 2);
	this->vbo3->load(this->vao);	
}

void Cilindro::render(GLSLProgram* programaHandler, glm::mat4 viewMat, glm::mat4 modelMat, glm::mat4 perspMat){
	
	/*
	//Activo el handler
	programaHandler->use();

	//Cargo matrices de modelo, view, proyeccion y normal
	programaHandler->setUniform("viewMatrix", viewMat);
	programaHandler->setUniform("modelMatrix", modelMat);
	programaHandler->setUniform("perspMatrix", perspMat);

	glm::mat4 VM = viewMat * modelMat;
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
	
	//Seteo el material
	programaHandler->setUniform("mat1.Ka", material.Ka);
	programaHandler->setUniform("mat1.Kd", material.Kd);
	programaHandler->setUniform("mat1.Ks", material.Ks);
	programaHandler->setUniform("mat1.brillo", material.brillo);
	
	//Asigno texturas
	if(this->isTextureDifusa)
		texturaDifusa.assignToShader(programaHandler, "texturaDataDifusa", 0);
	if(this->isTextureNormal)
		texturaNormal.assignToShader(programaHandler, "texturaDataNormal", 1);
	if(this->isTextureRefelexion){
		texturaReflexion.assignToShader(programaHandler, "texturaDataReflexion", 2);	
		programaHandler->setUniform("AlphaReflexion", this->factorAlpha);
		texturaReflexion.assignToShader(programaHandler, "texturaDataReflexion", 2);	
		programaHandler->setUniform("AlphaReflexion", this->factorAlpha);
	}
	programaHandler->setUniform("normalIntensity", this->normalIntensity);

	GraphicDevice::getInstance()->drawTriangleStrip(this->vao->getHandler(), sphere_index_buffer, index_buffer.size());
	*/
}


void Cilindro::setAlphaReflexion(float alpha){
	this->factorAlpha = alpha;
}

void Cilindro::setNormalIntensity(float alpha){
	this->normalIntensity = alpha;
}

void Cilindro::setMaterial(float Ka, float Kd, float Ks, float brillo){

	this->material.Ka = Ka;
	this->material.Kd = Kd;
	this->material.Ks = Ks;
	this->material.brillo = brillo;
}

void Cilindro::setMaterial(Material mat){

	this->material.Ka = mat.Ka;
	this->material.Kd = mat.Kd;
	this->material.Ks = mat.Ks;
	this->material.brillo = mat.brillo;
}


