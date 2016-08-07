/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "LineManager.h"
#include "LineShader.h"
#include "GraphicDevice.h"

LineManager* LineManager::instance = NULL;
unsigned int staticDraw = NULL;
unsigned int floatType = NULL;

LineManager::LineManager(){
	staticDraw = GraphicDevice::getInstance()->getConstantCatalog()->getStaticDrawFlag();
	floatType = GraphicDevice::getInstance()->getConstantCatalog()->getFloatTypeFlag();
}

int LineManager::addLine(vec3 start, vec3 end, vec3 color){
	Line* line = new Line();
	line->color = color;

	line->vertexBuffer[0] = start.x;
	line->vertexBuffer[1] = start.y;
	line->vertexBuffer[2] = start.z;

	line->vertexBuffer[3] = end.x;
	line->vertexBuffer[4] = end.y;
	line->vertexBuffer[5] = end.z;
	
	line->vaoObject = new VertexArrayObject();
	line->vboObject = new VertexBufferObject(line->vertexBuffer, 6, 3);  // 3 componentes, 2 puntos, data length = 6
	line->vboObject->load(line->vaoObject);	

	this->lines.push_back(line);
	return this->lines.size() - 1;
}

void LineManager::removeLine(int handler){
	int index = 0;
	for(std::vector<Line*>::iterator it=this->lines.begin(); it != this->lines.end(); it++){
		if(index == handler){
			delete (*it);
			this->lines.erase(it);
			break;
		}
		index++;
	}
}
void LineManager::setPosition(vec3 start, vec3 end, int handler){
	if(handler < this->lines.size()){
		Line* line = this->lines.at(handler);
		float* vertexBuffer = line->vertexBuffer;
		vertexBuffer[0] = start.x;
		vertexBuffer[1] = start.y;
		vertexBuffer[2] = start.z;

		vertexBuffer[3] = end.x;
		vertexBuffer[4] = end.y;
		vertexBuffer[5] = end.z;	
		
		GraphicArrayContainer container = GraphicArrayContainer(line->vboObject->getHandler(), 6, line->vertexBuffer, staticDraw, floatType, 3, line->vaoObject);
		GraphicDevice::getInstance()->modifyGeometryArray(&container, line->vboObject->getHandler());
	}
}
void LineManager::setColor(vec3 color, int handler){
	if(handler < this->lines.size())
		this->lines.at(handler)->color = color;	
}

void LineManager::renderLine(GLSLProgram* shader, mat4 viewMatrix, mat4 perspMatrix, Line* line){
	LineShader::getInstance()->draw(line->vaoObject->getHandler(), viewMatrix, perspMatrix, line->color); 
}

void LineManager::renderLine(GLSLProgram* shader, mat4 viewMatrix, mat4 perspMatrix, int lineHandler){	
	if(lineHandler < this->lines.size()){
		this->renderLine(shader, viewMatrix, perspMatrix, this->lines.at(lineHandler));
	}
}

void LineManager::render(GLSLProgram* shader, mat4 viewMatrix, mat4 perspMatrix){
	for(std::vector<Line*>::iterator it=this->lines.begin(); it != this->lines.end(); it++){
		this->renderLine(shader, viewMatrix, perspMatrix, (*it));
	}
}

LineManager* LineManager::getInstance(){
	if(instance == NULL)
		instance = new LineManager();
	return instance;
}