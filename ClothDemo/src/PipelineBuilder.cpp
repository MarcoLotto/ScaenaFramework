/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "PipelineBuilder.h"
#include "RenderPipelineWithGeometryAndBackWrite.h"
#include "UserInterfaceStage.h"

RenderPipeline* PipelineBuilder::getRenderPipelineConfigForward(UIController* uiController, Scene* scene){
	// Configuro el render pipeline
	RenderPipelineWithGeometryAndBackWrite* renderPipelineImp = new RenderPipelineWithGeometryAndBackWrite();

	// Inicializamos una etapa de geometria forward
	ForwardGeometryStage* geometryStage = getGeometryForwardStage(uiController, scene, renderPipelineImp);
	
	// Asignamos los stage al pipeline
	renderPipelineImp->addGeometryStage(geometryStage);

	// Terminado el pipeline, lo mando a cargar
	renderPipelineImp->updatePipelineScheme();  
	return renderPipelineImp;
}

// El forward geometry stage se utiliza para renderizar geometria en una sola etapa (es decir la iluminacion se produce en el mismo shader de geometria)
ForwardGeometryStage* PipelineBuilder::getGeometryForwardStage(UIController* uiController, Scene* scene, RenderPipelineWithGeometryAndBackWrite* pipeline){
	vec2 size = uiController->getScreenSize();
	ForwardGeometryStage* geometryStage = new ForwardGeometryStage(size.x, size.y, false);
	geometryStage->setScene(scene);  // Le seteo la escena a renderizar al geometry stage
	scene->getLightingManager()->getShadowManager()->setRenderPipeline(pipeline);
	return geometryStage;
}

RenderPipeline* PipelineBuilder::getRenderPipelineForUIOnly(UIController* uiController){
	// Configuro el render pipeline
	RenderPipelineWithGeometryAndBackWrite* renderPipelineImp = new RenderPipelineWithGeometryAndBackWrite();

	// Instanciamos una etapa de renderizado de UI para mostrar arriba de todo la interfaz de usuario
	vec2 size = uiController->getScreenSize();
	UserInterfaceStage* userInterfaceStage = new UserInterfaceStage(size.x, size.y, true);
	userInterfaceStage->setUiController(uiController);

	// Asignamos los stages al pipeline
	renderPipelineImp->addAfterGeometry(userInterfaceStage);

	// Terminado el pipeline, lo mando a cargar
	renderPipelineImp->updatePipelineScheme();  
	return renderPipelineImp;
}