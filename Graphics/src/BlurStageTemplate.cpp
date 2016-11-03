/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "BlurStageTemplate.h"
#include "GraphicDevice.h"

#define ITERATIONS 3

BlurStageTemplate::BlurStageTemplate(){
	this->firstFrameBuffer = NULL;
	this->secondFrameBuffer = NULL;
	this->fboRequest1 = NULL;
	this->previousStage = NULL;
}

BlurStageTemplate::~BlurStageTemplate(){
	if(this->fboRequest1 != NULL)
		delete this->fboRequest1;
}

// Agrega al vector de inputs los inputs de este Render Stage particular
void BlurStageTemplate::addStageInputs(list<OutputResource*>* inputs){
	inputs->push_back(this->previousStage->getBackBuffer());
}

FrameBufferRequest* BlurStageTemplate::configureFBORequest(int width, int height){
	GraphicDevice* device = GraphicDevice::getInstance();
	GraphicDeviceConstantCatalog* catalog = device->getConstantCatalog();

	//Genero el FrameBufferRequest
	FrameBufferRequest* request = new FrameBufferRequest();
	
	//Mantener con GL_LINEAR para que los post efectos que vengan despues hagan la mitad de fetchs
	TextureRequest* positionRequest = new TextureRequest(vec2(width, height), catalog->getFormatRGB(), catalog->getFormatRGB32F());
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMinFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMagFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapSFlag(), catalog->getTextureClampToEdgeFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapTFlag(), catalog->getTextureClampToEdgeFlag()));
	request->addTextureRequest(positionRequest);
	return request;
}

//****************************************
// Calculos de factores del filtro de blur
//****************************************

void BlurStageTemplate::initWeights(float blurFactor){
	const int auxWeightSize = 5;
	//Primero calculo gauss como si fuesen 5 pesos
	this->weightSum = 0;	
	float auxWeights[5];
	auxWeights[0] = this->gauss(0, blurFactor);
	this->weightSum = auxWeights[0];
	for(int i=1; i<auxWeightSize; i++){
		auxWeights[i] = this->gauss(i, blurFactor);
		this->weightSum += 2 * auxWeights[i];		
	}
	//Los dejo con el modulo correcto
	for(int i=0; i<auxWeightSize; i++){
		auxWeights[i] = auxWeights[i] / this->weightSum;;
	}
	//Creo los pesos finales como 3 pesos
	this->weights[0] = auxWeights[0];
	this->weights[1] = auxWeights[1] + auxWeights[2];
	this->weights[2] = auxWeights[3] + auxWeights[4];
	
	//Inicializo los positionIncrement
	this->posIncrement[0] = 0.0f;
	this->posIncrement[1] = (1.0f * auxWeights[1] +  2.0f * auxWeights[2]) / this->weights[1];
	this->posIncrement[2] = (3.0f * auxWeights[3] +  4.0f * auxWeights[4]) / this->weights[2];	
}

void BlurStageTemplate::changeBlurFactor(float sigma){
	this->initWeights(sigma);
}

float BlurStageTemplate::gauss(int x, float sigma){
	float e = 2.718281828459045f;
	float pi = 3.14159265358979f;

	float exp = -(x*x)/(2.0f*sigma*sigma);
	exp = glm::pow(e,exp);
	float term1 = 1.0f / (glm::sqrt(2.0f * pi * sigma*sigma));
	return term1 * exp;
}
