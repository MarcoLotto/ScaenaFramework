/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "CustomizableTexture.h"
#include "WindowConnector.h"
#include "ImageColorChangerShader.h"
#include "FileTexture.h"
#include "GraphicDevice.h"
#include "FrameBufferManager.h"
#include "ProgramParameters.h"

#define COLOR_CHANGE_TOLERANCE  0.1f

CustomizableTexture::CustomizableTexture(string textureInitialName){
	this->textureInitialName = textureInitialName;
	this->customizedTexture = NULL;
	this->outputTextureUsesAlphaChannel = false;
}

CustomizableTexture::~CustomizableTexture(){
	this->deleteCustomTexture();
}

void CustomizableTexture::addReplacementColor(vec3 rgbInputColor, vec3 rgbOutputColor){
	this->rgbInputColors.push_back(rgbInputColor);
	this->rgbOutputColors.push_back(rgbOutputColor);

	// Si hay una textura ya creada, la borramos porque se cambio la informacion de entrada
	this->deleteCustomTexture();
}

Texture* CustomizableTexture::getCustomizedTexture(){
	if(this->customizedTexture == NULL){
		this->buildCustomizedTexture();
	}
	return customizedTexture;
}

void CustomizableTexture::resetCustomization(){
	this->rgbInputColors.clear();
	this->rgbOutputColors.clear();
	this->deleteCustomTexture();
}

void CustomizableTexture::deleteCustomTexture(){
	// Borramos la textura generada
	if(this->customizedTexture != NULL){
		delete this->customizedTexture;
	}
	this->customizedTexture = NULL;
}

void CustomizableTexture::setTextureFilenameId(string filenameId){ 
	if(this->textureFilenameId.compare(filenameId) != 0){
		this->textureFilenameId = filenameId; 

		// Si hay una textura ya creada, la borramos porque se cambio la informacion de entrada
		this->deleteCustomTexture();
	}
}

void CustomizableTexture::buildCustomizedTexture(){	
	// Cargamos la textura de template a ser modificar
	FileTexture* templateTexture = new FileTexture(this->getTextureFilename().c_str());
	if(rgbInputColors.size() == 0 || rgbOutputColors.size() == 0){
		this->customizedTexture = templateTexture;
		return;
	}
	// Preparamos el framebuffer para escribir la nueva imagen customizada (usamos un frame buffer unmanaged, borraremos la textura desde aca)
	FrameBuffer* originalFbo = FrameBufferManager::getInstance()->getActiveFrameBuffer();
	FrameBufferRequest* request = this->configureFBORequest(templateTexture->getSize().x, templateTexture->getSize().y);
	FrameBuffer* unmanagedFrameBuffer = FrameBufferManager::getInstance()->getUnmanagedFrameBufferAndBind(request);
	GraphicDevice::getInstance()->setViewport(0,0, templateTexture->getSize().x, templateTexture->getSize().y);
	GraphicDevice::getInstance()->clearColorBuffer();

	ImageColorChangerShader::getInstance()->draw(templateTexture, &this->rgbInputColors, &this->rgbOutputColors, COLOR_CHANGE_TOLERANCE);	
	originalFbo->bind();

	// Guardamos la imagen customizada y terminamos
	this->customizedTexture = unmanagedFrameBuffer->getOutputTextures()->at(0);
	delete request;
	delete templateTexture;
	delete unmanagedFrameBuffer;
}

string CustomizableTexture::getTextureFilename(){
	return TEXTURES_DIFFUSE_DIR + this->textureInitialName + this->textureFilenameId + ".png";
}

FrameBufferRequest* CustomizableTexture::configureFBORequest(int width, int height){
	GraphicDevice* device = GraphicDevice::getInstance();
	GraphicDeviceConstantCatalog* catalog = device->getConstantCatalog();

	//Genero el FrameBufferRequest
	FrameBufferRequest* request = new FrameBufferRequest();
	
	//Mantener con GL_LINEAR para que los post efectos hagan la mitad de fetchs
	//Mantener GL_CLAMP_TO_EDGE para que no se vea el otro borde al aplicar blur
	int outputInternalFormat = catalog->getFormatRGB32F() * (1 - this->outputTextureUsesAlphaChannel) + catalog->getFormatRGBA() * this->outputTextureUsesAlphaChannel;
	int outputExternalFormat = catalog->getFormatRGB() * (1 - this->outputTextureUsesAlphaChannel) + catalog->getFormatRGBA() * this->outputTextureUsesAlphaChannel;
	TextureRequest* positionRequest = new TextureRequest(vec2(width, height), outputExternalFormat, outputInternalFormat);
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMinFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMagFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapSFlag(), catalog->getTextureClampToEdgeFlag()));
	positionRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapTFlag(), catalog->getTextureClampToEdgeFlag()));
	request->addTextureRequest(positionRequest);
	return request;
}