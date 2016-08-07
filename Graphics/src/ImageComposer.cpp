/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ImageComposer.h"
#include "UIShader.h"
#include "GraphicDevice.h"
#include "FrameBufferManager.h"

ImageComposer::ImageComposer(vec2 finalImageSize){
	this->fboRequest = this->configureFBORequest(finalImageSize.x, finalImageSize.y);
	this->finalImageSize = finalImageSize;
}

ImageComposer::~ImageComposer(){
	list<CompositionPacket*>::iterator it = this->texturesToCompose.begin();
	while(it != this->texturesToCompose.end()){
		delete (*it);
		it = this->texturesToCompose.erase(it);
	}
	if(this->fboRequest != NULL){
		delete this->fboRequest;
	}
}
	
Texture* ImageComposer::makeComposition(){	
	FrameBuffer* originalFbo = FrameBufferManager::getInstance()->getActiveFrameBuffer();
	FrameBuffer* frameBuffer = FrameBufferManager::getInstance()->getUnmanagedFrameBufferAndBind(this->fboRequest);
	GraphicDevice::getInstance()->enableMode(GraphicDevice::getInstance()->getConstantCatalog()->getBlendFlag());	
	GraphicDevice::getInstance()->setViewport(0, 0, this->finalImageSize.x, this->finalImageSize.y);
	GraphicDevice::getInstance()->clearColorAndDepthBuffer();

	list<CompositionPacket*>::iterator it = this->texturesToCompose.begin();
	while(it != this->texturesToCompose.end()){
		CompositionPacket* packet = *it;
		mat4 scale = glm::scale(mat4(1.0f), vec3(packet->scale.x, packet->scale.y, 1.0f));
		mat4 translation = glm::translate(mat4(1.0f), vec3(packet->position.x, packet->position.y, 0.0f));
		mat4 finalMatrix = translation * scale;
		UIShader::getInstance()->drawSideCenteredCuad(packet->texture, vec2(1.0f), vec2(0.0f), finalMatrix, packet->transparency);	
		++it;
	}
	GraphicDevice::getInstance()->disableMode(GraphicDevice::getInstance()->getConstantCatalog()->getBlendFlag());	
	originalFbo->bind();

	Texture* finalTexture = frameBuffer->getOutputTextures()->at(0);
	delete frameBuffer;  // Es unmanaged, hay que borrarlo
	return finalTexture;
}

void ImageComposer::addImage(Texture* texture, vec2 position, vec2 scale, float transparency){
	CompositionPacket* packet = new CompositionPacket();
	packet->texture = texture;
	packet->position = position;
	packet->scale = scale;
	packet->transparency = transparency;
	this->texturesToCompose.push_back(packet);
}

FrameBufferRequest* ImageComposer::configureFBORequest(int width, int height){
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