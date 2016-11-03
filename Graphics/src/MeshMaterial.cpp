/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "MeshMaterial.h"
#include "Mesh.h"

MeshMaterial::MeshMaterial(){
	this->ambient = 1.0f;
	this->diffuse = 0.0f;
	this->specular = 0.0f;
	this->bright = 100.0f;

	// Identifico los shaders
	this->kaUniform = new GLSLUniform("mat.Ka");
	this->kdUniform = new GLSLUniform("mat.Kd");
	this->ksUniform = new GLSLUniform("mat.Ks");
	this->kbUniform = new GLSLUniform("mat.Kb");
}
MeshMaterial::MeshMaterial(float ambient, float diffuse, float specular, float bright){
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->bright = bright;

	// Identifico los shaders
	this->kaUniform = new GLSLUniform("mat.Ka");
	this->kdUniform = new GLSLUniform("mat.Kd");
	this->ksUniform = new GLSLUniform("mat.Ks");
	this->kbUniform = new GLSLUniform("mat.Kb");
}

MeshMaterial::~MeshMaterial(){
	delete this->kaUniform;
	delete this->kdUniform;
	delete this->ksUniform;
	delete this->kbUniform;
}
void MeshMaterial::setAmbient(float factor){
	this->ambient = factor;
}
void MeshMaterial::setDiffuse(float factor){
	this->diffuse = factor;
}
void MeshMaterial::setSpecular(float factor){
	this->specular = factor;
}
void MeshMaterial::setBright(float factor){
	this->bright = factor;
}
	
float MeshMaterial::getAmbient(){
	return this->ambient;
}
float MeshMaterial::getDiffuse(){
	return this->diffuse;
}
float MeshMaterial::getSpecular(){
	return this->specular;
}
float MeshMaterial::getBright(){
	return this->bright;
}

void MeshMaterial::chargeIntoShader(GLSLProgram* shader, unsigned int textureUnit){
	if(this->kaUniform->getGlslProgram() != shader){
		this->kaUniform->setGlslProgram(shader);
		this->kdUniform->setGlslProgram(shader);
		this->ksUniform->setGlslProgram(shader);
		//this->kbUniform->setGlslProgram(shader);
	}
	shader->setUniform(this->kaUniform, this->ambient);
	shader->setUniform(this->kdUniform, this->diffuse);
	shader->setUniform(this->ksUniform, this->specular);
	//shader->setUniform(this->kbUniform, this->bright);  //TODO: Habilitar cuando se habilite en shaders
}

void MeshMaterial::clone(Mesh* cloneMesh){
	MeshMaterial* clone = cloneMesh->getMaterial();
	clone->setAmbient(this->ambient);
	clone->setDiffuse(this->diffuse);
	clone->setSpecular(this->specular);
	clone->setBright(this->bright);
}


