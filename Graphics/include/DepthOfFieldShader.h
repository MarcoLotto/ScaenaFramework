/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GLSLProgram.h"

class Texture;

typedef glm::vec2 vec2;

#define MAX_ITERATIONS 5

class DepthOfFieldShader : public GLSLProgram{

private:
	static DepthOfFieldShader* instance;

	GLSLUniform* pvmMatrixUniform;
	GLSLUniform* imageSizeUniform;
	GLSLUniform* textureToBlurUniform;	
	GLSLUniform* depthTextureUniform;
	GLSLUniform* sceneProjUniform;
	GLSLUniform* depthAtBlurStartUniform;
	GLSLUniform* blurToFrontUniform;
	GLSLUniform* blurFalloffUniform;
	GLSLUniform* weightUniform;
	GLSLUniform* positionIncrementUniform;	

protected:
	Mesh* rectangleMesh;
	mat4 PVMmatrix;
	float weights[MAX_ITERATIONS];
	float sumOfWeightsPerIteration[MAX_ITERATIONS];

	void initProyectionMatrix();
	
public:
	static DepthOfFieldShader* getInstance();
	
	// depthAtBlurStart es de donde empieza a actual el depth of field, blurToFront indica si se aplica blur opuesto a la camara o hacia la camara, blurFalloff es que tan rapido cae el blur
	virtual void drawFirstPass(Texture* textureToBlur, Texture* depthTexture, vec2 imageSize, float* weights, float* posIncrement, float depthAtBlurStart, 
								bool blurToFront, mat4 sceneProj, float blurFalloff);
	virtual void drawSecondPass(Texture* textureToBlur, Texture* depthTexture, vec2 imageSize, float* weights, float* posIncrement, float depthAtBlurStart, 
								bool blurToFront, mat4 sceneProj, float blurFalloff);
	virtual void init();
};
