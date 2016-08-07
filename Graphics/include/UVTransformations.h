/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ChargeableResource.h"
#include <glm/glm.hpp> 

class Mesh;

typedef glm::vec2 vec2;

class UVTransformations : public ChargeableResource{

private:
	vec2 uvScale;
	vec2 uvTranslation;

	GLSLUniform* uvTranslationUniform;
	GLSLUniform* uvScaleUniform;

public:
	UVTransformations();
	virtual ~UVTransformations();

	void setUvScale(vec2 scale){ this->uvScale = scale;	}
	void setUvTranslation(vec2 translation){ this->uvTranslation = translation; }
	vec2 getUvScale(){ return this->uvScale; }
	vec2 getUvTranslation(){ return this->uvTranslation; }

	void chargeIntoShader(GLSLProgram* shader, unsigned int textureUnit);
	void clone(Mesh* cloneMesh);
};
