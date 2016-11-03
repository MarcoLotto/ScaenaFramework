/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "RenderStage.h"
#include "OutputResource.h"
#include "Scene.h"

class GeometryStage : virtual public RenderStage{

protected:
	Scene* scene;
	
public:
	GeometryStage() : RenderStage(){}

	// Define el nombre del stage. Esto se utiliza para identificar al stage por ejemplo en el profiling
	virtual string getName() = 0;

	// Define sus outputs
	virtual void configureStageOutputs() = 0;

	// Renderiza el render stage actual
	virtual void render(bool isTheLastStage) = 0;

	// Agrega al vector de inputs los inputs de este Render Stage particular
	virtual void addStageInputs(list<OutputResource*>* inputs) = 0;

	// Actualiza la informacion de las texturas de salida
	virtual void updateOutputData() = 0;
	
	// Por defecto, un GeometryStage debe tener como salida un depth buffer
	virtual OutputResource* getDepthBuffer() = 0;

	// Setea la escena en el Stage
	void setScene(Scene* scene){ this->scene = scene; }	
	Scene* getScene(){ return this->scene; }	
};