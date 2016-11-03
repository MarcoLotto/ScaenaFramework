/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Texture.h"
#include "TextureFilter.h"
#include <map>
using namespace std;

class MemoryTexture : public Texture{

private:
	bool writeLock;
	map<unsigned int, unsigned int> appliedFilters;
	void saveFilter(unsigned int type, unsigned int value);

protected:
	virtual void deleteTextureFromMemory()=0;
	
public:
	MemoryTexture();
	virtual ~MemoryTexture();

	//Aplica un filtro especifico a la textura (ej min y max filters)
	void applyFilter(unsigned int filterType, unsigned int value);
	void applyFilter(unsigned int filterType, float* valueArray, unsigned int arrayLenght);

	//Identifica si un determinado tipo de filtro esta aplicado
	bool isFilterTypeApplied(unsigned int type){ return (this->appliedFilters.count(type) > 0); }
	//Consigue el valor de un filtro
	unsigned int getAppliedFilter(unsigned int type){ return this->appliedFilters.at(type); }

	//No me gusta que exponga su id pero no veo por ahora otra manera rapida de bindear a un framebuffer
	unsigned int getTextureId(){ return this->textureId; }

	// Indica si tiene o no activado el bloqueo de escritura
	void setWriteLock(bool lock){ 
		this->writeLock = lock; 
	}
	bool hasWriteLock(){ return this->writeLock; }

	// Consigue un entero que representa al hash de un array de float. Es usado para verificar que dos filtros sean iguales.
	static unsigned int generateValueArrayHashCode(float* valueArray, unsigned int arrayLength);

	virtual Texture* clone() = 0;

};