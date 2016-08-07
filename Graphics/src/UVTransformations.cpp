/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UVTransformations.h"
#include "Mesh.h"

UVTransformations::UVTransformations(){
	this->uvTranslation = vec2(0.0f);
	this->uvScale = vec2(1.0f);

	// Identifico los shaders
	this->uvTranslationUniform = new GLSLUniform("uvTranslation");
	this->uvScaleUniform = new GLSLUniform("uvScale");	
}

UVTransformations::~UVTransformations(){
	delete this->uvTranslationUniform;
	delete this->uvScaleUniform;
}

void UVTransformations::chargeIntoShader(GLSLProgram* shader, unsigned int textureUnit){
	if(this->uvTranslationUniform->getGlslProgram() != shader){
		this->uvTranslationUniform->setGlslProgram(shader);
		this->uvScaleUniform->setGlslProgram(shader);
	}
	shader->setUniform(this->uvTranslationUniform, this->uvTranslation);
	shader->setUniform(this->uvScaleUniform, this->uvScale);
}

void UVTransformations::clone(Mesh* cloneMesh){
	UVTransformations* clone = cloneMesh->getUVTransformations();
	clone->setUvTranslation(this->uvTranslation);
	clone->setUvScale(this->uvScale);
}
