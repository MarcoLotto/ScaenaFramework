#include "DeferredRenderPipelineTest.h"
#include "RenderPipelineBuilder.h"

void DeferredRenderPipelineTest::when(){
	UIController* uiController = new UIController();
	uiController->setScreenSize(vec2(300, 300));
	Scene* scene = new Scene();
	Camara* camera = new CamaraPrimeraPersona();
	scene->setCurrentCamera(camera);
	RenderPipeline* pipeline = RenderPipelineBuilder::getRenderPipelineConfigDeferred(uiController, scene);
	pipeline->render();
}

void DeferredRenderPipelineTest::then(MockGraphicDevice* device){
	this->assertDeviceAccess(device, "bindFrameBuffer", 3);
	this->assertDeviceAccess(device, "bindTexture", 20);
	this->assertDeviceAccess(device, "bindTextureToFrameBuffer", 3);
	this->assertDeviceAccess(device, "clearColorAndDepthBuffer", 1);
	this->assertDeviceAccess(device, "disableMode", 2);
	this->assertDeviceAccess(device, "clearColorBuffer", 1);
	this->assertDeviceAccess(device, "drawFrameBuffer", 2);
	this->assertDeviceAccess(device, "drawMesh", 1);
	this->assertDeviceAccess(device, "generateFrameBuffer", 1);
	this->assertDeviceAccess(device, "generateTexture", 4);
	this->assertDeviceAccess(device, "setActiveTexture", 4);
	this->assertDeviceAccess(device, "setTextureParameterFor2DTexture", 16);
	this->assertDeviceAccess(device, "setViewport", 2);
}