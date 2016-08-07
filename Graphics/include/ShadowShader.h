/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "MeshDrawerShader.h"

class ShadowShader : public MeshDrawerShader{

private:
	static ShadowShader* instance;

	GLSLUniform* pvmMatrixUniform;

protected:
	void prepareForDraw(Mesh* mesh);

public:
	static ShadowShader* getInstance();	
	void init();
};
