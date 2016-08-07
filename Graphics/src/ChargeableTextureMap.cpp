/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ChargeableTextureMap.h"
#include "FileTexture.h"

ChargeableTextureMap::ChargeableTextureMap(){
	this->mapTexture = NULL;
	this->mapFilename = "";
}

// Se le setea la textura, pero pasandole el filename. Esto es para que cada
// chargeableTextureMap tenga su propia textura y pueda ser borrada junto con el
void ChargeableTextureMap::setMap(const char* filename){ 
	if(this->mapTexture != NULL){
		delete this->mapTexture;
	}
	this->mapFilename = string(filename);
	this->mapTexture = new FileTexture(filename);
}

void ChargeableTextureMap::setMap(Texture* texture){ 
	if(this->mapTexture != NULL){
		delete this->mapTexture;
	}
	this->mapTexture = texture->clone();
	this->mapFilename = "";
}

void ChargeableTextureMap::setMap(string filename){
	this->mapFilename = filename;
	this->setMap(filename.c_str());
}

ChargeableTextureMap::~ChargeableTextureMap(){ 
	if(this->mapTexture != NULL)
		delete this->mapTexture; 
}

string ChargeableTextureMap::getMapFilename(){
	// Si es un FileTexture, conseguimos su nombre de textura y la guardamos por si se repregunta
	if(this->mapFilename.empty() && this->mapTexture != NULL){
		FileTexture* fileTexture = dynamic_cast<FileTexture*>(this->mapTexture);
		if(fileTexture != NULL){
			this->mapFilename = fileTexture->getFilename();	
		}
	}
	return this->mapFilename;
}