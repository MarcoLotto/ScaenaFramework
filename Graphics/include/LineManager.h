/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GLSLProgram.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "LineShader.h"
#include <vector>
using namespace std;

class LineManager{
	
private:
	LineManager();

	struct Line{
		float vertexBuffer[6];
		VertexArrayObject* vaoObject;
		VertexBufferObject* vboObject;
		vec3 color;
	};

	static LineManager* instance;

	vector<Line*> lines;
	void renderLine(GLSLProgram* shader, mat4 viewMatrix, mat4 perspMatrix, Line* line);
public:
	static LineManager* getInstance();

	//***Acceso a las lineas*****
	int addLine(vec3 start, vec3 end, vec3 color);
	void removeLine(int handler);
	void setPosition(vec3 start, vec3 end, int handler);	
	void setColor(vec3 color, int handler);
	//***************************

	void renderLine(GLSLProgram* shader, mat4 viewMatrix, mat4 perspMatrix, int lineHandler);
	void render(GLSLProgram* shader, mat4 viewMatrix, mat4 perspMatrix);
};