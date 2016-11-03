/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "VideoConfiguration.h"
#include "GraphicDevice.h"
#include <iostream>
#include <fstream>

//*******************************************************************
//*  				      Valores default						    *
//*******************************************************************
//Antialising de texturas (anisotropy y mipmap)
#define VCONF_ANISOTROPY_LEVEL 16
#define VCONF_ANISOTROPY_ACTIVE true
#define VCONF_MIPMAP_ACTIVE true

// La resolucion apra la que se considera que se toma la informada por la pantalla
#define REAL_SCREEN_RESOLUTION vec2(-1.0f)

//Correccion gamma
#define GAMMA_CORRECTION 0.6f

//Modo debug o modo release (debug mode off tiene menos validaciones pero
//es mas performante, para un release es mejor en off)
#define DEBUG_MODE true
//******************************************************************

VideoConfiguration* VideoConfiguration::instance = NULL;

VideoConfiguration* VideoConfiguration::getInstance(){
	if(instance == NULL){
		instance = new VideoConfiguration();
		instance->runGlslVersionTest();
		instance->initiateDefaultValues();
	}
	return instance;
}

//Aca se configuran todos los valores por default de video al comenzar la aplicación
void VideoConfiguration::initiateDefaultValues(){
	this->setAnisotropyLevel(VCONF_ANISOTROPY_LEVEL);
	this->setAnisotropyActive(VCONF_ANISOTROPY_ACTIVE);
	this->setMipMapActive(VCONF_MIPMAP_ACTIVE);	

	this->setScreenResolution(REAL_SCREEN_RESOLUTION);
	this->setGammaCorrection(GAMMA_CORRECTION);
	this->setDebugMode(DEBUG_MODE);
}

bool VideoConfiguration::isScreenResolutionEqualsToRealResolution(){
	return (this->screenResolution.x <= 0.0f || this->screenResolution.y <= 0.0f); 
}

//Se determina la maxima versión de GLSL que soporta el sistema
void VideoConfiguration::runGlslVersionTest(){
	this->setGlsl400Avaible(false);
	unsigned char* version = GraphicDevice::getInstance()->getApiVersion();
	std::cout << "Using GLSL Version: " << version << std::endl;
	if(version[0] == '4' || version[0] == '5'){
		this->setGlsl400Avaible(true);
	}	
	else if(version[0] == '2'){
		//No esta soportado
		exit(1);
	}
}