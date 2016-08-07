/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <stdio.h>
#include "glm/glm.hpp"
typedef glm::vec2 vec2;

class VideoConfiguration{

private:
	static VideoConfiguration* instance;

	bool glsl400Avaible;
	bool mipMapActive;
	bool anisotropyActive;
	int anisotropyLevel;
	float gammaCorrection;
	bool debugMode;
	vec2 screenResolution;
	
public:
	static VideoConfiguration* getInstance();	

	void setGlsl400Avaible(bool value){ this->glsl400Avaible = value;};	
	void setMipMapActive(bool value){ this->mipMapActive = value;};
	void setAnisotropyActive(bool value){ this->anisotropyActive = value;};
	void setAnisotropyLevel(int value){ this->anisotropyLevel = value;};
	void setGammaCorrection(float value){ this->gammaCorrection = value;};
	void setDebugMode(bool active){ this->debugMode = active; }
	void setScreenResolution(vec2 resolution){ this->screenResolution = resolution; }

	bool isGlsl400Avaible(){ return this->glsl400Avaible;};	
	bool isMipMapActive(){ return this->mipMapActive;};
	bool isAnisotropyActive(){ return this->anisotropyActive;};
	int getAnisotropyLevel(){ return this->anisotropyLevel;};
	float getGammaCorrection(){ return this->gammaCorrection;};
	float isDebugMode(){ return this->debugMode;};

	// Si no se setea una resolucion particular se define que es igual a la de la pantalla. Si no buscarla en el metodo getScreenResolution
	bool isScreenResolutionEqualsToRealResolution();
	vec2 getScreenResolution(){ return this->screenResolution; }

private:
	void initiateDefaultValues();
	void runGlslVersionTest();
		
};