/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "FrameBufferRequest.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <vector>
using namespace std;

typedef glm::vec3 vec3;

class CustomizableTexture{

private:
	Texture* customizedTexture;

	string textureInitialName;
	string textureFilenameId;
	bool outputTextureUsesAlphaChannel;

	vector<vec3> rgbInputColors;
	vector<vec3> rgbOutputColors;

	void buildCustomizedTexture();
	string getTextureFilename();

	FrameBufferRequest* configureFBORequest(int width, int height);

public:
	CustomizableTexture(string textureInitialName);
	virtual ~CustomizableTexture();

	// Agrega un color para reemplazar en la imagen
	void addReplacementColor(vec3 rgbInputColor, vec3 rgbOutputColor);	
		
	// Devuelve la imagen customizada
	Texture* getCustomizedTexture();

	// Borra la textura customizada, limpia todos los parametros de colores y permite customizar nuevamente
	void resetCustomization();

	// Borra la informacion de textura customizada para liberar recursos, pero no borra las configuraciones, por lo que se lo puede crear otra vez con getObject()
	void deleteCustomTexture();

	// Determina si la textura de salida tendra componente alpha (por defecto no tiene)
	void setOutputTextureUsesAlphaChannel(bool value){ this->outputTextureUsesAlphaChannel = value; }
		
	void setTextureFilenameId(string filenameId);
	string getTextureFilenameId(){ return this->textureFilenameId; }

	vector<vec3>* getRgbInputColors(){ return &this->rgbInputColors; }
	vector<vec3>* getRgbOutputColors(){ return &this->rgbOutputColors; }
};