/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ApiTexture.h"
#include "DepthTexture.h"
#include "FrameBufferRequest.h"
#include "TimeManager.h"
#include <vector>
using namespace std;

class FrameBuffer{

private:
	void lockOutputTextures();

protected:
	vector<ApiTexture*>* outputTextures;
	MemoryTexture* depthBuffer;
	unsigned int usedTimeSlot;
	unsigned int fboHandler;
	unsigned int hashCode;

	void addTextureInformationToList(MemoryTexture* texture, list<unsigned int>* textureInformation);
	void generateHashCode();

public:
	// Crea un frame buffer vacio
	FrameBuffer();
	virtual ~FrameBuffer();

	// Activa el frame buffer actual	
	virtual void bind();
	virtual void bindAndLockOutputs();

	// Devuelve las texturas en las que escribe este frame buffer
	vector<ApiTexture*>* getOutputTextures();
	// Agrega una textura a la escritura de este frame buffer
	void addOutputTexture(ApiTexture* texture);
	
	// Asocia un depth buffer por su id o handler (si solo se quiere para evaluacion de depth)
	void attachDepthBuffer(DepthTexture* depthBuffer);
	// Asocia un depth buffer por su id o handler (si se quiere usar el depth buffer como textura despues)
	void attachDepthBuffer(ApiTexture* depthBuffer);

	MemoryTexture* getDepthBuffer(){ return this->depthBuffer; }
	
	// Identifica si tiene write lock (esto sucede si alguna de sus
	// texturas tiene write lock)
	bool hasWriteLock();

	// Setea el frame buffer handler 0, que imprime en el back buffer
	void setAsScreenFrameBuffer();

	// Devuelve el tiempo que paso desde el 
	unsigned int getElapsedTimeSinceBinding();

	// Devuelve el hash code del frame buffer para identificar la composicion del mismo
	unsigned int getHashCode(){ return this->hashCode; }

	// Compara el hash code pasado por parametro con el hash code de este frame buffer
	bool areHashCodesEqual(unsigned int otherHashCode){ return (this->hashCode == otherHashCode); }
};