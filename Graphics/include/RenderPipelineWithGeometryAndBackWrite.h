/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "RenderPipeline.h"
#include "GeometryStage.h"
#include "BackBufferWriterStage.h"

class RenderPipelineWithGeometryAndBackWrite : public RenderPipeline{

protected:
	list<RenderStage*> backWritersBeforeGeometry;
	list<RenderStage*> geometryStages;
	list<RenderStage*> backWritersAfterGeometry;

	virtual void configurePipelineScheme();

public:
	RenderPipelineWithGeometryAndBackWrite();
	virtual ~RenderPipelineWithGeometryAndBackWrite();

	// Agrega un stage de back writer(post efectos) antes de las etapas de geometry
	void addBeforeGeometry(BackBufferWriterStage* stage);
	// Agrega una etapa de renderizado de geometrias
	void addGeometryStage(GeometryStage* stage);
	// Agrega un stage de back writer(light y post efectos) despues de las etapas de geometry
	void addAfterGeometry(BackBufferWriterStage* stage);	

	// Remueve un stage de la etapa indicada. Hace falta volver a cargar el pipeline para que haga efecto.
	void removeBeforeGeometry(BackBufferWriterStage* stage){ this->backWritersBeforeGeometry.remove(stage); }
	void removeGeometryStage(GeometryStage* stage){ this->geometryStages.remove(stage); }
	void removeAfterGeometry(BackBufferWriterStage* stage){ this->backWritersAfterGeometry.remove(stage); }
};