/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "PhongLightingStage.h"
#include "GraphicDevice.h"
#include "FrameBufferManager.h"
#include "PhongLightingShader.h"
#include "PhongLightingWithSSAOShader.h"

PhongLightingStage::PhongLightingStage(DeferredGeometryStage* geometryStage, int screenWidth, int screenHeight, BackBufferWriterStage* ssaoStage){
	this->frameBuffer = NULL;
	this->fboRequest = this->configureFBORequest(screenWidth, screenHeight);
	this->configureStageOutputs();
	this->geometryStage = geometryStage;
	this->scene = NULL;
	this->screenSize = vec2(screenWidth, screenHeight);
	this->ssaoStage = ssaoStage;
}

PhongLightingStage::~PhongLightingStage(){
	delete this->fboRequest;
}
	
// Actaliza la informacion de las texturas de sus outputs
void PhongLightingStage::configureStageOutputs(){
	this->addOutputResource(new OutputResource(NULL));  // La escena iluminada
}

// Renderiza el render stage actual
void PhongLightingStage::render(bool isTheLastStage){
	// Cambio el viewport al corecto segun el tamaño de imagen
	GraphicDevice::getInstance()->setViewport(0,0, this->screenSize.x, this->screenSize.y);

	this->frameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequest, this->frameBuffer, isTheLastStage);

	GraphicDevice::getInstance()->clearColorBuffer();
	GraphicDeviceConstantCatalog* apiConstants = GraphicDevice::getInstance()->getConstantCatalog();
	GraphicDevice::getInstance()->disableMode(apiConstants->getDepthTestFlag());	
	GraphicDevice::getInstance()->enableMode(apiConstants->getCullFaceFlag());	

	//********Renderizo el rectangulo que contendra la imagen de pantalla**********
	glm::vec3 escalaImagen = glm::vec3(2.0f, -2.0f, -2.0f);

	glm::mat4 proyeccionOrtogonal = glm::mat4(1.0f);
	proyeccionOrtogonal[2][2] = 0.0f;	
	glm::mat4 model = glm::scale(glm::mat4(1.0f), escalaImagen);

	Texture* depth = this->geometryStage->getDepthBuffer()->getOutputTexture();
	Texture* normal = this->geometryStage->getNormalBuffer()->getOutputTexture();
	Texture* difuse = this->geometryStage->getDifuseBuffer()->getOutputTexture();
	Texture* material = this->geometryStage->getMaterialBuffer()->getOutputTexture();
	
	Camara* camera = this->scene->getCurrentCamera();

	if(this->ssaoStage == NULL){
		PhongLightingShader* shader = PhongLightingShader::getInstance();
		shader->prepareForDraw(depth, normal, difuse, material);
		shader->draw(this->scene->getLightingManager(), camera->getViewMatrix(), camera->getPerspectiveMatrix(), model, proyeccionOrtogonal);  
	}
	else{
		PhongLightingWithSSAOShader* shader = PhongLightingWithSSAOShader::getInstance();
		Texture* ssaoData = this->ssaoStage->getBackBuffer()->getOutputTexture();
		shader->prepareForDraw(depth, normal, difuse, material, ssaoData);
		shader->draw(this->scene->getLightingManager(), camera->getViewMatrix(), camera->getPerspectiveMatrix(), model, proyeccionOrtogonal); 
	}
	//*****************************************************************************

	GraphicDevice::getInstance()->disableMode(apiConstants->getCullFaceFlag());	
	GraphicDevice::getInstance()->enableMode(apiConstants->getDepthTestFlag());	
}

// Agrega al vector de inputs los inputs de este Render Stage particular
void PhongLightingStage::addStageInputs(list<OutputResource*>* inputs){
	inputs->push_back(this->geometryStage->getDepthBuffer());
	inputs->push_back(this->geometryStage->getNormalBuffer());
	inputs->push_back(this->geometryStage->getDifuseBuffer());
	inputs->push_back(this->geometryStage->getMaterialBuffer());

	if(this->ssaoStage != NULL){
		inputs->push_back(this->ssaoStage->getBackBuffer());
	}
}

void PhongLightingStage::updateOutputData(){
	this->updateOutputResource(this->frameBuffer->getOutputTextures()->at(0), 0); // La escena iluminada
}

FrameBufferRequest* PhongLightingStage::configureFBORequest(int width, int height){
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

