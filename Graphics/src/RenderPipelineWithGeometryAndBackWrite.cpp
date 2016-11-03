/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "RenderPipelineWithGeometryAndBackWrite.h"

RenderPipelineWithGeometryAndBackWrite::RenderPipelineWithGeometryAndBackWrite(){
}
RenderPipelineWithGeometryAndBackWrite::~RenderPipelineWithGeometryAndBackWrite(){
}

// Agrega un stage de back writer(post efectos) antes de las etapas de geometry
void RenderPipelineWithGeometryAndBackWrite::addBeforeGeometry(BackBufferWriterStage* stage){
	this->backWritersBeforeGeometry.push_back(stage);
}
// Agrega una etapa de renderizado de geometrias
void RenderPipelineWithGeometryAndBackWrite::addGeometryStage(GeometryStage* stage){
	this->geometryStages.push_back(stage);
}
// Agrega un stage de back writer(light y post efectos) despues de las etapas de geometry
void RenderPipelineWithGeometryAndBackWrite::addAfterGeometry(BackBufferWriterStage* stage){
	this->backWritersAfterGeometry.push_back(stage);
}

void RenderPipelineWithGeometryAndBackWrite::configurePipelineScheme(){
	this->addAllStagesToPipeline(&this->backWritersBeforeGeometry);
	this->addAllStagesToPipeline(&this->geometryStages);
	this->addAllStagesToPipeline(&this->backWritersAfterGeometry);
}


