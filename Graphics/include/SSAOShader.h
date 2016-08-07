/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GLSLProgram.h"
#include "ApiTexture.h"

typedef glm::vec2 vec2;

class SSAOShader : public GLSLProgram{

private:
	static SSAOShader* instance;
	static bool hasBeenInstanced;
	mat4 PVMmatrix;

	GLSLUniform* pvmMatrixUniform;
	GLSLUniform* imageSizeUniform;
	GLSLUniform* depthTexUniform;	
	GLSLUniform* sceneProjUniform;
	GLSLUniform* sampleKernelUniform;
	GLSLUniform* samplesCountUniform;
	GLSLUniform* randomTextureUniform;
	GLSLUniform* randomTextureSizeUniform;
	GLSLUniform* normalTexUniform;	

protected:
	float* sampleKernel;
	ApiTexture* randomTexture;

	float random(float a, float b);
	void generateRandomTexture();
	float* getSampleKernel(unsigned int samplesCount);
	Mesh* rectangleMesh;
	void initProyectionMatrix();
	void orderKernelByDistanceToOrigin(vector<vec3>* unorderedSamplePoints);

	
public:
	static SSAOShader* getInstance();

	SSAOShader();
	virtual ~SSAOShader();
	
	virtual void draw(Texture* positionTexture, Texture* normalTexture, unsigned int fetchsPerFragment, vec2 imageSize, mat4 sceneProjection);
	virtual void init();
};