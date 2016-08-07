/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ChargeableResource.h"

class Mesh;

class MeshMaterial : public ChargeableResource{

private:
	float ambient;
	float diffuse;
	float specular;
	float bright;

	GLSLUniform* kaUniform;
	GLSLUniform* kdUniform;
	GLSLUniform* ksUniform;
	GLSLUniform* kbUniform;

public:
	MeshMaterial();
	MeshMaterial(float ambient, float diffuse, float specular, float bright);
	virtual ~MeshMaterial();
	
	void setAmbient(float factor);
	void setDiffuse(float factor);
	void setSpecular(float factor);
	void setBright(float factor);
	
	float getAmbient();
	float getDiffuse();
	float getSpecular();
	float getBright();

	void chargeIntoShader(GLSLProgram* shader, unsigned int textureUnit);
	void clone(Mesh* cloneMesh);
};