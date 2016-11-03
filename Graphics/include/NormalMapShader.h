/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshDrawerShaderImplProxy.h"

// Shader para normal map comun
class NormalMapShader : public MeshDrawerShaderImplProxy{

private:
	static NormalMapShader* instance;

protected:
	virtual MeshDrawerShader* configureDeferredShadingImplementation();
	virtual MeshDrawerShader* configureForwardShadingImplementation();
		
public:
	static NormalMapShader* getInstance();	
};