/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshDrawerShaderImplProxy.h"

// Shader para normal map con skinned mesh
class SkinnedNormalMapShader : public MeshDrawerShaderImplProxy{

private:
	static SkinnedNormalMapShader* instance;

protected:
	virtual MeshDrawerShader* configureDeferredShadingImplementation();
	virtual MeshDrawerShader* configureForwardShadingImplementation();
		
public:
	static SkinnedNormalMapShader* getInstance();	
};