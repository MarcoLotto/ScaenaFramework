/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "MemoryTexture.h"
#include "GraphicDevice.h"

MemoryTexture::MemoryTexture() : Texture(){
	this->writeLock = false;
}

MemoryTexture::~MemoryTexture(){
}

void MemoryTexture::saveFilter(unsigned int type, unsigned int value){
	//Si ya existe el filtro, actualizo el valor, sino lo creo
	this->appliedFilters[type] = value;	
}

//Aplica un filtro especifico a la textura (ej min y max filters)
void MemoryTexture::applyFilter(unsigned int filterType, unsigned int value){
	if(this->isTextureValid()){
		GraphicDevice::getInstance()->bindTexture(this->textureId);
		GraphicDevice::getInstance()->setTextureParameterFor2DTexture(filterType, value);
		this->saveFilter(filterType, value);		
	}
}

void MemoryTexture::applyFilter(unsigned int filterType, float* valueArray, unsigned int arrayLenght){
	if(this->isTextureValid()){
		GraphicDevice::getInstance()->bindTexture(this->textureId);
		GraphicDevice::getInstance()->setTextureParameterFor2DTexture(filterType, valueArray);
		// Guardo el tipo de filtro junto con el hash del array
		unsigned int arrayHashCode = this->generateValueArrayHashCode(valueArray, arrayLenght);
		this->saveFilter(filterType, arrayHashCode);		
	}
}

unsigned int MemoryTexture::generateValueArrayHashCode(float* valueArray, unsigned int arrayLength){
	// La funcion de hash es la siguiente, divido cada elemento del array por su orden en el array, asi evito que
	// dos convinaciones en diferente orden pero con los mismos numeros sean tratados como iguales. Sumo cada elemento.
	// Consigo el modulo al tamaño de un unsigned int en cada suma.
	unsigned int valueArrayHashCode = 0;
	for(int i=0; i < arrayLength; i++){
		valueArrayHashCode += (unsigned int) (valueArray[i] / (float) i) % sizeof(unsigned int);
	}
	return valueArrayHashCode;
}