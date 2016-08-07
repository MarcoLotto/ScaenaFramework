/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GLSLUniform.h"

class LightUniform{

private:
	GLSLUniform* positionUniform;
	GLSLUniform* directionUniform;
	GLSLUniform* intensityUniform;
	GLSLUniform* spotFactorUniform;
	GLSLUniform* shadowMapUniform;
	GLSLUniform* shadowMatrixUniform;
	GLSLUniform* shadowIndexUniform;

public:
	LightUniform(int lightNumber, GLSLProgram* glslProgram);
	virtual ~LightUniform();

	GLSLUniform* getPositionUniform(){ return this->positionUniform; }
	GLSLUniform* getDirectionUniform(){ return this->directionUniform; }
	GLSLUniform* getIntensityUniform(){ return this->intensityUniform; }
	GLSLUniform* getSpotFactorUniform(){ return this->spotFactorUniform; }
	GLSLUniform* getShadowMapUniform(){ return this->shadowMapUniform; }
	GLSLUniform* getShadowMatrixUniform(){ return this->shadowMatrixUniform; }
	GLSLUniform* getShadowIndexUniform(){ return this->shadowIndexUniform; }
};