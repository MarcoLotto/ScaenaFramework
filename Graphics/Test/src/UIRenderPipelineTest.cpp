#include "UIRenderPipelineTest.h"
#include "RenderPipelineBuilder.h"

void UIRenderPipelineTest::when(){
	UIController* uiController = new UIController();
	uiController->setScreenSize(vec2(300, 300));
	Scene* scene = new Scene();
	RenderPipeline* pipeline = RenderPipelineBuilder::getRenderPipelineConfigForwardAndUI(uiController, scene);
	pipeline->render();
}

void UIRenderPipelineTest::then(MockGraphicDevice* device){
	this->assertDeviceAccess(device, "attachDepthBufferToFrameBufferAsRenderBuffer", 1);
	this->assertDeviceAccess(device, "bindFrameBuffer", 3);
	this->assertDeviceAccess(device, "bindTexture", 5);
	this->assertDeviceAccess(device, "bindTextureToFrameBuffer", 1);
	this->assertDeviceAccess(device, "clearColorAndDepthBuffer", 1);
	this->assertDeviceAccess(device, "disableMode", 2);
	this->assertDeviceAccess(device, "clearColorBuffer", 1);
	this->assertDeviceAccess(device, "drawFrameBuffer", 2);
	this->assertDeviceAccess(device, "drawMesh", 1);
	this->assertDeviceAccess(device, "generateDepthBuffer", 1);
	this->assertDeviceAccess(device, "generateFrameBuffer", 1);
	this->assertDeviceAccess(device, "generateTexture", 1);
	this->assertDeviceAccess(device, "setActiveTexture", 1);
	this->assertDeviceAccess(device, "setTextureParameterFor2DTexture", 4);
	this->assertDeviceAccess(device, "setViewport", 2);
}