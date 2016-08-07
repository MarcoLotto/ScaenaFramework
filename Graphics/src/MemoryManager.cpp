/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "MemoryManager.h"
#include <stddef.h>

MemoryManager* MemoryManager::instance = NULL;


//**************************General excecution**************************
//**********************************************************************+
MemoryManager* MemoryManager::getInstance(){
	if(instance == NULL)
		instance = new MemoryManager();	
	return instance;
}

void MemoryManager::freeAllBuffers(){
	//Libero los buffers de punto flotante
	for(int i=0; i < this->bufferListPF.size(); i++){
		EntradaPF *entrada = this->bufferListPF.back();
		delete entrada->buffer;
		delete entrada;
		this->bufferListPF.pop_back();
	}
	//Libero los buffers de enteros
	for(int i=0; i < this->bufferListINT.size(); i++){
		EntradaINT *entrada = this->bufferListINT.back();
		delete entrada->buffer;
		delete entrada;
		this->bufferListINT.pop_back();
	}
}

//**************************Punto flotante******************************
//**********************************************************************+
bool MemoryManager::compararBuffer(EntradaPF* entrada1, EntradaPF* entrada2){
	if(entrada1->tamanio != entrada2->tamanio)
		return false;
	for(int i=0; i<entrada1->tamanio ; i++){
		if(entrada1->buffer[i] != entrada2->buffer[i])
			return false;
	}
	return true;
}
int MemoryManager::hashFunction(float* buffer, int tam){
	int clave = 0;
	for(int i=0; i < tam; i++){
		if(clave >= CLAVE_MAX){
			clave = clave % CLAVE_MAX;
		}
		clave += (int)buffer[i];
	}
	return clave;
}


float* MemoryManager::checkAndGetBuffer(float* buffer, int *tam){
	EntradaPF* entrada =new EntradaPF;
	entrada->clave = this->hashFunction(buffer, *tam);
	entrada->buffer = buffer;
	entrada->tamanio = *tam;
	
	for(int i=0; i < this->bufferListPF.size(); i++){
		if(this->bufferListPF[i]->clave == entrada->clave){
			if(this->compararBuffer(entrada, this->bufferListPF[i])){
				delete buffer;  //Libero memoria y retorno
				buffer = NULL;
				*tam = this->bufferListPF[i]->tamanio;
				return this->bufferListPF[i]->buffer;
			}
		}
	}
	//No se encontro la clave, procedo a guardarla	
	this->bufferListPF.push_back(entrada);
	return buffer;
}

//**************************Enteros*************************************
//**********************************************************************+
bool MemoryManager::compararBuffer(EntradaINT* entrada1, EntradaINT* entrada2){
	if(entrada1->tamanio != entrada2->tamanio)
		return false;
	for(int i=0; i<entrada1->tamanio ; i++){
		if(entrada1->buffer[i] != entrada2->buffer[i])
			return false;
	}
	return true;
}

int MemoryManager::hashFunction(unsigned int* buffer, int tam){
	int clave = 0;
	for(int i=0; i < tam; i++){
		if(clave >= CLAVE_MAX){
			clave = clave % CLAVE_MAX;
		}
		clave += (int)buffer[i];
	}
	return clave;
}


unsigned int* MemoryManager::checkAndGetBuffer(unsigned int* buffer, int *tam){
	EntradaINT* entrada =new EntradaINT;
	entrada->clave = this->hashFunction(buffer, *tam);
	entrada->buffer = buffer;
	entrada->tamanio = *tam;
	
	for(int i=0; i < this->bufferListINT.size(); i++){
		if(this->bufferListINT[i]->clave == entrada->clave){
			if(this->compararBuffer(entrada, this->bufferListINT[i])){
				delete buffer;  //Libero memoria y retorno
				buffer = NULL;
				*tam = this->bufferListINT[i]->tamanio;
				return this->bufferListINT[i]->buffer;
			}
		}
	}
	//No se encontro la clave, procedo a guardarla	
	this->bufferListINT.push_back(entrada);
	return buffer;
}

