/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "BoundingBox.h"
#include "LineManager.h"
#include "LineShader.h"

// Permite dibjar en pantalla al bounding box (como una linea a travez de sus dos vertices principales)
class DrawableBoundingBox : public BoundingBox{

private:
	int lineHandler;

public:
	DrawableBoundingBox(){
		this->lineHandler = LineManager::getInstance()->addLine(this->absMinPoint, this->absMinPoint, vec3(1.0f, 0.0f, 0.0f));
	}
	virtual ~DrawableBoundingBox(){
		LineManager::getInstance()->removeLine(this->lineHandler);
	}

	void draw(mat4 viewMatrix, mat4 proyMatrix){
		LineManager::getInstance()->setPosition(this->absMinPoint, this->absMaxPoint, this->lineHandler);  // Tremendamente no performante
		LineManager::getInstance()->renderLine(LineShader::getInstance(), viewMatrix, proyMatrix, this->lineHandler);
	}
};