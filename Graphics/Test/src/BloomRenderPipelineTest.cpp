#include "BloomRenderPipelineTest.h"
#include "RenderPipelineBuilder.h"

void BloomRenderPipelineTest::when(){
	UIController* uiController = new UIController();
	uiController->setScreenSize(vec2(300, 300));
	Scene* scene = new Scene();
	RenderPipeline* pipeline = RenderPipelineBuilder::getRenderPipelineConfigForwardAndBloom(uiController, scene);
	pipeline->render();
}

void BloomRenderPipelineTest::then(MockGraphicDevice* device){
	this->assertDeviceAccess(device, "attachDepthBufferToFrameBufferAsRenderBuffer", 1);
	this->assertDeviceAccess(device, "bindFrameBuffer", 7);
	this->assertDeviceAccess(device, "bindTexture", 16);
	this->assertDeviceAccess(device, "bindTextureToFrameBuffer", 3);
	this->assertDeviceAccess(device, "clearColorAndDepthBuffer", 2);
	this->assertDeviceAccess(device, "clearColorBuffer", 2);
	this->assertDeviceAccess(device, "drawFrameBuffer", 6);
	this->assertDeviceAccess(device, "drawMesh", 3);
	this->assertDeviceAccess(device, "generateDepthBuffer", 1);
	this->assertDeviceAccess(device, "generateFrameBuffer", 3);
	this->assertDeviceAccess(device, "generateTexture", 3);
	this->assertDeviceAccess(device, "getApiVersion", 1);
	this->assertDeviceAccess(device, "setActiveTexture", 4);
	this->assertDeviceAccess(device, "setTextureParameterFor2DTexture", 12);
	this->assertDeviceAccess(device, "setViewport", 2);
}