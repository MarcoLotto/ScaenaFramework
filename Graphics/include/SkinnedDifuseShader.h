/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshDrawerShaderImplProxy.h"

// Shader para difuse shader con skinned shader
class SkinnedDifuseShader : public MeshDrawerShaderImplProxy{

private:
	static SkinnedDifuseShader* instance;

protected:
	virtual MeshDrawerShader* configureDeferredShadingImplementation();
	virtual MeshDrawerShader* configureForwardShadingImplementation();

public:
	static SkinnedDifuseShader* getInstance();	
};
