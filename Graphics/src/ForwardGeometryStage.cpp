/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ForwardGeometryStage.h"
#include "GraphicDevice.h"
#include "FrameBufferManager.h"
#include "ShaderManager.h"
#include "FeatureNotAvaibleException.h"

void ForwardGeometryStage::initClass(int screenWidth, int screenHeight, bool manageDepthAsTexture, int outTextureExternalFormat, int outTextureInternalFormat){
	this->frameBuffer = NULL;
	this->fboRequest = this->configureFBORequest(screenWidth, screenHeight, manageDepthAsTexture, outTextureExternalFormat, outTextureInternalFormat);
	this->configureStageOutputs();
	this->scene = NULL;
	this->screenSize = vec2(screenWidth, screenHeight);
	this->blendingEnabled = false;
    this->forceRenderToScreen = false;
}

ForwardGeometryStage::ForwardGeometryStage(int screenWidth, int screenHeight, bool manageDepthAsTexture){
	GraphicDeviceConstantCatalog* catalog = GraphicDevice::getInstance()->getConstantCatalog();
	this->initClass(screenWidth, screenHeight, manageDepthAsTexture, catalog->getFormatRGB(), catalog->getFormatRGB32F());
}

ForwardGeometryStage::ForwardGeometryStage(int screenWidth, int screenHeight, bool manageDepthAsTexture, int outTextureExternalFormat, int outTextureInternalFormat){
	this->initClass(screenWidth, screenHeight, manageDepthAsTexture, outTextureExternalFormat, outTextureInternalFormat);
}


ForwardGeometryStage::~ForwardGeometryStage(){
	delete this->fboRequest;
}
	
// Actaliza la informacion de las texturas de sus outputs
void ForwardGeometryStage::configureStageOutputs(){
    if(this->manageDepthAsTexture){
        this->addOutputResource(new OutputResource(NULL));  // El depth de la escena
    }
	this->addOutputResource(new OutputResource(NULL));  // La escena final, renderizada e iluminada
}

// Renderiza el render stage actual
void ForwardGeometryStage::render(bool isTheLastStage){
	// Cambio el viewport al corecto segun el tamaño de imagen
	GraphicDevice::getInstance()->setViewport(0,0, this->screenSize.x, this->screenSize.y);

	// Si se solicito, habilito el blending
	GraphicDeviceConstantCatalog* catalog = GraphicDevice::getInstance()->getConstantCatalog();
	if(this->blendingEnabled){		
		GraphicDevice::getInstance()->enableMode(catalog->getBlendFlag());	
		GraphicDevice::getInstance()->blendFunction(catalog->getBlendSrcAlphaFlag(), catalog->getBlendOneMinusSrcAlphaFlag());
	}
	// Indico a los shaders que se debe utilizar la estrategia de renderizado de forward shading y le indico la escena que utilizo al shader manager (por la iluminacion)
	ShaderManager::getInstance()->useForwardShadingStrategy();
	ShaderManager::getInstance()->setCurrentScene(this->scene);

	this->frameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(this->fboRequest, this->frameBuffer, isTheLastStage || this->forceRenderToScreen);
	GraphicDevice::getInstance()->clearColorAndDepthBuffer();

	this->scene->update();
	this->scene->renderFromCurrentCamera();
}

// Agrega al vector de inputs los inputs de este Render Stage particular
void ForwardGeometryStage::addStageInputs(list<OutputResource*>* inputs){
	//No tiene inputs
}

void ForwardGeometryStage::updateOutputData(){
    if(this->manageDepthAsTexture){
        this->updateOutputResource(this->frameBuffer->getDepthBuffer(), 0);			   // Depth de la escena
        this->updateOutputResource(this->frameBuffer->getOutputTextures()->at(0), 1); // La escena final, renderizada e iluminada
    }
    else{
        this->updateOutputResource(this->frameBuffer->getOutputTextures()->at(0), 0); // La escena final, renderizada e iluminada
    }
}

FrameBufferRequest* ForwardGeometryStage::configureFBORequest(int width, int height, bool manageDepthAsTexture, int outTextureExternalFormat, int outTextureInternalFormat){
    this->manageDepthAsTexture = manageDepthAsTexture;
	GraphicDevice* device = GraphicDevice::getInstance();
	GraphicDeviceConstantCatalog* catalog = device->getConstantCatalog();

	//Genero el FrameBufferRequest
	FrameBufferRequest* request = new FrameBufferRequest();


    TextureRequest* depthRequest = NULL;
    if(this->manageDepthAsTexture){
        // Pido una textura de depth de 24 bits de profundidad para usar posteriormente (no solo para depth evaluation). Mantener el min y mag filter en Nearest!
        depthRequest = new TextureRequest(vec2(width, height), catalog->getFormatDepth(), catalog->getFormatDepth24());
        depthRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMinFilterFlag(), catalog->getTextureNearestFilterFlag()));
        depthRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMagFilterFlag(), catalog->getTextureNearestFilterFlag()));
        depthRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapSFlag(), catalog->getTextureClampToEdgeFlag()));
        depthRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapTFlag(), catalog->getTextureClampToEdgeFlag()));
    }
    else{
        // Si no queremos reutilizar el depth en post-efectos, simplemente lo creamos sin format interno (no podremos accederlo como textura)
        depthRequest = new TextureRequest(vec2(width, height), catalog->getFormatDepth(), 0);
    }
	request->setDepthBufferRequest(depthRequest);
	
	//Mantener con GL_LINEAR para que los post efectos hagan la mitad de fetchs
	//Mantener GL_CLAMP_TO_EDGE para que no se vea el otro borde al aplicar blur
	TextureRequest* positionRequest = new TextureRequest(vec2(width, height), outTextureExternalFormat, outTextureInternalFormat);
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMinFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMagFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapSFlag(), catalog->getTextureClampToEdgeFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapTFlag(), catalog->getTextureClampToEdgeFlag()));
	request->addTextureRequest(positionRequest);

	return request;
}

OutputResource* ForwardGeometryStage::getBackBuffer(){
    if(this->manageDepthAsTexture){
        return this->getOutputs()->at(1);
    }
    return this->getOutputs()->at(0);
}

OutputResource* ForwardGeometryStage::getDepthBuffer(){
    if(this->manageDepthAsTexture){
        return this->getOutputs()->at(0);
    }
    throw new FeatureNotAvaibleException("El Forward Stage se configuro para no manejar el depth como textura, el mismo no puede obtenerse para post-efectos");
}

