/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "GraphicDevice.h"
#include "VideoConfiguration.h"
#include "TextureLoader.h"
#include "PngTextureLoader.h"
#include "TextureLoadException.h"
#include <string.h>
using namespace std;

#define FORMAT_PNG "PNG"
#define FORMAT_JPG "JPG"

void TextureLoader::applyTextureFilters(TextureDataTransfer* textureData){
	GraphicDevice* device = GraphicDevice::getInstance();
	GraphicDeviceConstantCatalog* catalog = device->getConstantCatalog();
	VideoConfiguration* videoConfig = VideoConfiguration::getInstance();		
	//Aplico filtro anisotropico
	if(videoConfig->isAnisotropyActive()){
		device->setTextureParameterFor2DTexture(catalog->getTextureAnisotropyFlag(), videoConfig->getAnisotropyLevel()); 		
	}
	//Activo mipmaps
	if(videoConfig->isMipMapActive()){
		device->setTextureParameterFor2DTexture(catalog->getTextureMagFilterFlag(), catalog->getTextureLinearFilteringFlag());
		device->setTextureParameterFor2DTexture(catalog->getTextureMinFilterFlag(), catalog->getTextureMipmapLinearFilteringFlag());
		device->generateMipMapFor2DTexture(textureData->width, textureData->height);
	}
	//Default
	else{
		device->setTextureParameterFor2DTexture(catalog->getTextureMagFilterFlag(), catalog->getTextureLinearFilteringFlag());
		device->setTextureParameterFor2DTexture(catalog->getTextureMinFilterFlag(), catalog->getTextureLinearFilteringFlag());	
	}
}

TextureDataTransfer* TextureLoader::parseTextureFile(const char* filename){
	string fileType = this->getExtension(filename);
	if(fileType.compare(FORMAT_PNG) == 0)
		return PngTextureLoader::loadTexture(filename);	
	return NULL;
	//throw new TextureLoadException("El formato de textura no soportado: " + fileType);
}

unsigned int TextureLoader::parseFileAndLoadInMemory(const char* filename){	
	// Load texture file	
	TextureDataTransfer* textureData = this->parseTextureFile(filename);
	if(textureData == NULL ){
		return 0;     //REVIEW!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}

	// Defino el formato interno para el API
	unsigned int textureFormat = 0;
	if(textureData->numberOfChannels == 1){
		textureFormat = GraphicDevice::getInstance()->getConstantCatalog()->getFormatR();
		this->textureInternalFormat = GraphicDevice::getInstance()->getConstantCatalog()->getFormatR8();
	}
	else if(textureData->numberOfChannels == 2){
		textureFormat = GraphicDevice::getInstance()->getConstantCatalog()->getFormatRG();
		this->textureInternalFormat = GraphicDevice::getInstance()->getConstantCatalog()->getFormatRG8();
	}
	else if(textureData->numberOfChannels == 3){
		textureFormat = GraphicDevice::getInstance()->getConstantCatalog()->getFormatRGB();
		this->textureInternalFormat = GraphicDevice::getInstance()->getConstantCatalog()->getFormatRGB8();
	}
	else if(textureData->numberOfChannels >= 4){
		textureFormat = GraphicDevice::getInstance()->getConstantCatalog()->getFormatRGBA();
		this->textureInternalFormat = GraphicDevice::getInstance()->getConstantCatalog()->getFormatRGBA8();
	}
    
	// Reservo memoria, pido id, y guardo la textura
	unsigned int textureId = GraphicDevice::getInstance()->generateTexture(vec2(textureData->width, textureData->height), this->textureInternalFormat, 
			textureFormat, textureData->data);
		
	//Aplico los filtros como mipmapping, anisotropico, bilineal, etc, segun corresponda
	this->applyTextureFilters(textureData);

	//Guardo los datos
	this->textureSize = vec2(textureData->width, textureData->height);
	this->lastFilename = filename;

	// Ya no preciso mas la informacion de la textura en la memoria principal, la borro
	delete textureData;
	return textureId;
}

string TextureLoader::getExtension(const char *filename){
	int largoNombre = strlen(filename);
	char extension[4];
	extension[3] = 0;
	extension[2] = filename[largoNombre-1];
	extension[1] = filename[largoNombre-2];
	extension[0] = filename[largoNombre-3];	
	if((strcmp(extension, "png")==0)||(strcmp(extension, "PNG")==0))
		return FORMAT_PNG;
	else if((strcmp(extension, "jpg")==0)||(strcmp(extension, "JPG")==0))
		return FORMAT_JPG;
	return extension;
}

vec2 TextureLoader::getTextureSize(const char* filename){
	//Compruebo que la ultima textura parseada sea la buscada
	if(strcmp(filename, this->lastFilename.c_str()) != 0){		
		this->parseTextureFile(filename);		
	}
	return this->lastTextureSize;
}


	
