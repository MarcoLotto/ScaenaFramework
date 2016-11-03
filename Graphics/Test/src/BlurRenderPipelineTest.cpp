#include "BlurRenderPipelineTest.h"
#include "RenderPipelineBuilder.h"

void BlurRenderPipelineTest::when(){
	UIController* uiController = new UIController();
	uiController->setScreenSize(vec2(300, 300));
	Scene* scene = new Scene();
	RenderPipeline* pipeline = RenderPipelineBuilder::getRenderPipelineConfigForwardAndBlur(uiController, scene);
	pipeline->render();
}

void BlurRenderPipelineTest::then(MockGraphicDevice* device){
	this->assertDeviceAccess(device, "attachDepthBufferToFrameBufferAsRenderBuffer", 1);
	this->assertDeviceAccess(device, "bindFrameBuffer", 5);
	this->assertDeviceAccess(device, "bindTexture", 10);
	this->assertDeviceAccess(device, "bindTextureToFrameBuffer", 2);
	this->assertDeviceAccess(device, "clearColorAndDepthBuffer", 2);
	this->assertDeviceAccess(device, "clearColorBuffer", 1);
	this->assertDeviceAccess(device, "drawFrameBuffer", 4);
	this->assertDeviceAccess(device, "drawMesh", 2);
	this->assertDeviceAccess(device, "generateDepthBuffer", 1);
	this->assertDeviceAccess(device, "generateFrameBuffer", 2);
	this->assertDeviceAccess(device, "generateTexture", 2);
	this->assertDeviceAccess(device, "setActiveTexture", 2);
	this->assertDeviceAccess(device, "setTextureParameterFor2DTexture", 8);
	this->assertDeviceAccess(device, "setViewport", 2);
}