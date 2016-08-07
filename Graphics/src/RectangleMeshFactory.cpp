/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "RectangleMeshFactory.h"
#include "GraphicArrayContainer.h"
#include "GraphicDevice.h"

#define CANT_VERTICES 4

bool RectangleMeshFactory::centeredRectangleLoaded = false;
bool RectangleMeshFactory::uiRectangleLoaded = false;

Mesh* RectangleMeshFactory::createCenteredRectangleMesh(){
	Mesh* mesh = new Mesh();
	mesh->getMeshBuffers()->setVertexBuffer(positionData, 3*CANT_VERTICES);
	mesh->getMeshBuffers()->setTextureCoords(texturaDefaultData, 2*CANT_VERTICES);
	mesh->getMeshBuffers()->setIndexBuffer(indexData, 6);
	if(!centeredRectangleLoaded){
		mesh->getMeshBuffers()->load();
		centeredRectangleLoaded = true;
	}
	return mesh;
}

Mesh* RectangleMeshFactory::createUIRectangleMesh(){
	Mesh* mesh = new Mesh();
	mesh->getMeshBuffers()->setVertexBuffer(sidePositionData, 3*CANT_VERTICES);
	mesh->getMeshBuffers()->setTextureCoords(sideTextureDefaultData, 2*CANT_VERTICES);
	mesh->getMeshBuffers()->setIndexBuffer(indexData, 6);
	if(!uiRectangleLoaded){
		mesh->getMeshBuffers()->load();
		uiRectangleLoaded = true;
	}
	return mesh;
}

float RectangleMeshFactory::sidePositionData[] = 
{
	 1.0f, 1.0f, 0.0f,
	 1.0f, 0.0f, 0.0f,
	 0.0f, 0.0f, 0.0f,
	 0.0f, 1.0f, 0.0f
};

float RectangleMeshFactory::positionData[] = 
{	
	  0.5f, 0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f,
	 -0.5f, -0.5f, 0.0f,
	  -0.5f, 0.5f, 0.0f,
};

float RectangleMeshFactory::texturaDefaultData[] = 
{    
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f
};

float RectangleMeshFactory::sideTextureDefaultData[] = 
{
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f
};

unsigned int RectangleMeshFactory::indexData[]={ 0,1,2,
						   0,2,3 };

