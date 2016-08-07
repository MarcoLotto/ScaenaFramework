/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GLSLProgram.h"
#include <vector>
using namespace std;

class Texture;

typedef glm::vec2 vec2;

class ImageColorChangerShader : public GLSLProgram{

private:
	static ImageColorChangerShader* instance;
	mat4 PVMmatrix;
	ImageColorChangerShader(){};

	GLSLUniform* pvmMatrixUniform;
	GLSLUniform* imageUniform;
	GLSLUniform* colorsCountUniform;
	GLSLUniform* colorToleranceUniform;
	vector<GLSLUniform*> inputColorsUniform;
	vector<GLSLUniform*> outputColorsUniform;

protected:
	Mesh* rectangleMesh;
	void initProyectionMatrix();
	
public:
	static ImageColorChangerShader* getInstance();
	
	// Recibe como entrada la imagen template, los colores a cambiar y por cuales cambiarlos. Recibe además el colorTolerance que
	// es que tan alejado puede estar un color de la imagen respecto de inputColors para reemplazarlo por el color de output (ej . 0.0f es nada)
	virtual void draw(Texture* image, vector<vec3>* rgbInputColors, vector<vec3>* rgbOutputColors, float colorTolerance = 0.0f);
	virtual void init();
};