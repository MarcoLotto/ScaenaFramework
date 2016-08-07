/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshDrawerShaderImplProxy.h"

class ParallaxShader : public MeshDrawerShaderImplProxy{

private:
	static ParallaxShader* instance;

protected:
	virtual MeshDrawerShader* configureDeferredShadingImplementation();
	virtual MeshDrawerShader* configureForwardShadingImplementation();

public:
	static ParallaxShader* getInstance();

};