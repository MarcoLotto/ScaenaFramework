#include "DepthOfFieldRenderPipelineTest.h"
#include "RenderPipelineBuilder.h"

void DepthOfFieldRenderPipelineTest::when(){
	UIController* uiController = new UIController();
	uiController->setScreenSize(vec2(300, 300));
	Scene* scene = new Scene();
	Camara* camera = new CamaraPrimeraPersona();
	scene->setCurrentCamera(camera);
	RenderPipeline* pipeline = RenderPipelineBuilder::getRenderPipelineConfigDeferredAndDepthOfField(uiController, scene, true);
	pipeline->render();
}

void DepthOfFieldRenderPipelineTest::then(MockGraphicDevice* device){
	this->assertDeviceAccess(device, "bindFrameBuffer", 7);
	this->assertDeviceAccess(device, "bindTexture", 32);
	this->assertDeviceAccess(device, "bindTextureToFrameBuffer", 5);
	this->assertDeviceAccess(device, "clearColorAndDepthBuffer", 2);
	this->assertDeviceAccess(device, "disableMode", 2);
	this->assertDeviceAccess(device, "clearColorBuffer", 2);
	this->assertDeviceAccess(device, "drawFrameBuffer", 6);
	this->assertDeviceAccess(device, "drawMesh", 3);
	this->assertDeviceAccess(device, "generateFrameBuffer", 3);
	this->assertDeviceAccess(device, "generateTexture", 6);
	this->assertDeviceAccess(device, "setActiveTexture", 8);
	this->assertDeviceAccess(device, "setTextureParameterFor2DTexture", 24);
	this->assertDeviceAccess(device, "setViewport", 3);
}