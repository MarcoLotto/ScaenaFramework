/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshDrawerShaderImplProxy.h"

class SkinnedParallaxShader : public MeshDrawerShaderImplProxy{

private:
	static SkinnedParallaxShader* instance;

protected:
	virtual MeshDrawerShader* configureDeferredShadingImplementation();
	virtual MeshDrawerShader* configureForwardShadingImplementation();

public:
	static SkinnedParallaxShader* getInstance();

};