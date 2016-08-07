/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshDrawerShaderImplProxy.h"

// Shader para difuse shader comun
class DifuseShader : public MeshDrawerShaderImplProxy{

private:
	static DifuseShader* instance;

protected:
	virtual MeshDrawerShader* configureDeferredShadingImplementation();
	virtual MeshDrawerShader* configureForwardShadingImplementation();

public:
	static DifuseShader* getInstance();	
};
