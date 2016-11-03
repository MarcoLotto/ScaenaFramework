/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ChargeableResource.h"
#include "Texture.h"

class Mesh;

// Para aquellos recursos que tienen como salida al shader un mapa o textura
class ChargeableTextureMap : public ChargeableResource{

protected:
	Texture* mapTexture;
	string mapFilename;

public:
	ChargeableTextureMap();
	virtual ~ChargeableTextureMap();

	virtual void chargeIntoShader(GLSLProgram* shader, unsigned int textureUnit) = 0;
	virtual void clone(Mesh* cloneMesh) = 0;
	
	Texture* getMap(){ return this->mapTexture; }

	// Se le setea la textura, pasandole el filename o la textura a cargar. Tener en cuenta que cada
	// ChargeableTextureMap tendra su propia textura y la misma sera borrada al borrase el mismo o al cambiar la textura.
	void setMap(const char* filename);
	void setMap(string filename);
	void setMap(Texture* texture);

	// Devuelve el path a la textura del filename. Tener en cuenta que esto solo tiene validez de haberse cargado un FileTexture.
	string getMapFilename();
};