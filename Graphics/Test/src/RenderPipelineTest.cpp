#include "RenderPipelineTest.h"
#include "RenderPipelineWithGeometryAndBackWrite.h"
#include "UIController.h"
#include "ForwardGeometryStage.h"

ForwardGeometryStage* getGeometryForwardStage(UIController* uiController, Scene* scene, RenderPipelineWithGeometryAndBackWrite* pipeline){
	vec2 size = uiController->getScreenSize();
	ForwardGeometryStage* geometryStage = new ForwardGeometryStage(size.x, size.y, false);
	geometryStage->setScene(scene);  // Le seteo la escena a renderizar al geometry stage
	scene->getLightingManager()->getShadowManager()->setRenderPipeline(pipeline);
	return geometryStage;
}

RenderPipelineWithGeometryAndBackWrite* getRenderPipelineConfigForward(UIController* uiController, Scene* scene){
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

void RenderPipelineTest::when(){
	UIController* uiController = new UIController();
	uiController->setScreenSize(vec2(300, 300));
	Scene* scene = new Scene();
	RenderPipelineWithGeometryAndBackWrite* pipeline = getRenderPipelineConfigForward(uiController, scene);
	pipeline->render();
}

void RenderPipelineTest::then(MockGraphicDevice* device){
	this->assertEquals(device->getMethodCalls("generateFrameBuffer"), 1);
	this->assertEquals(device->getMethodCalls("attachDepthBufferToFrameBufferAsTexture"), 1);
	this->assertEquals(device->getMethodCalls("setViewport"), 1);
	this->assertEquals(device->getMethodCalls("bindFrameBuffer"), 1);
	this->assertEquals(device->getMethodCalls("clearColorAndDepthBuffer"), 1);
	this->assertEquals(device->getMethodCalls("generateVboBuffer"), 6);
	this->assertEquals(device->getMethodCalls("generateVaoBuffer"), 2);
}