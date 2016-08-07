/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SSAORenderStage.h"
#include "GraphicDevice.h"
#include "FrameBufferManager.h"

#define DEFAULT_FETCHS_PER_FRAGMENT_COUNT 16

SSAORenderStage::SSAORenderStage(DeferredGeometryStage* geometryStage, int screenWidth, int screenHeight){
	this->fboRequest = this->configureFBORequest(screenWidth, screenHeight);
	this->configureStageOutputs();
	this->geometryStage = geometryStage;
	this->screenSize = vec2(screenWidth, screenHeight);
	this->fetchsPerFragment = DEFAULT_FETCHS_PER_FRAGMENT_COUNT;

	// Ya creo los FBOs de SSAO, de esta forma tengo disponibles las texturas para leerlas (aunque esten vacias)
	this->frameBufferPass1 = NULL;
	this->ssaoShaderPass1 = NULL;
}

SSAORenderStage::~SSAORenderStage(){
	if(this->ssaoShaderPass1 != NULL){
		delete this->ssaoShaderPass1;
		this->ssaoShaderPass1 = NULL;
	}
}
	
// Actaliza la informacion de las texturas de sus outputs
void SSAORenderStage::configureStageOutputs(){
	this->addOutputResource(new OutputResource(NULL));  // La informacion de AO
}

// Renderiza el render stage actual
void SSAORenderStage::render(bool isTheLastStage){
	this->frameBufferPass1 = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequest, this->frameBufferPass1);

	// Cambio el viewport al corecto segun el tamaño de imagen
	GraphicDevice::getInstance()->setViewport(0,0, this->screenSize.x, this->screenSize.y);
	
	GraphicDevice::getInstance()->clearColorBuffer();
	
	Texture* positionTexture = this->geometryStage->getDepthBuffer()->getOutputTexture();
	Texture* normalTexture = this->geometryStage->getNormalBuffer()->getOutputTexture();
	mat4 sceneProjection = this->geometryStage->getScene()->getCurrentCamera()->getPerspectiveMatrix();
	SSAOShader::getInstance()->draw(positionTexture, normalTexture, this->fetchsPerFragment, this->screenSize, sceneProjection);
}

// Agrega al vector de inputs los inputs de este Render Stage particular
void SSAORenderStage::addStageInputs(list<OutputResource*>* inputs){
	inputs->push_back(this->geometryStage->getDepthBuffer());
	inputs->push_back(this->geometryStage->getNormalBuffer());
}

void SSAORenderStage::updateOutputData(){
	this->updateOutputResource(this->frameBufferPass1->getOutputTextures()->at(0), 0); // La informacion mergeada de AO
}

FrameBufferRequest* SSAORenderStage::configureFBORequest(int width, int height){
	GraphicDevice* device = GraphicDevice::getInstance();
	GraphicDeviceConstantCatalog* catalog = device->getConstantCatalog();

	//Genero el FrameBufferRequest
	FrameBufferRequest* request = new FrameBufferRequest();
	
	//Mantener con GL_LINEAR para que los post efectos hagan la mitad de fetchs
	//Mantener GL_CLAMP_TO_EDGE para que no se vea el otro borde al aplicar blur
	TextureRequest* positionRequest = new TextureRequest(vec2(width, height), catalog->getFormatRGB(), catalog->getFormatRGB32F());
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMinFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMagFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapSFlag(), catalog->getTextureClampToEdgeFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapTFlag(), catalog->getTextureClampToEdgeFlag()));
	request->addTextureRequest(positionRequest);
	return request;
}