#include "ForwardRenderPipelineTest.h"
#include "RenderPipelineBuilder.h"

void ForwardRenderPipelineTest::when(){
	UIController* uiController = new UIController();
	uiController->setScreenSize(vec2(300, 300));
	Scene* scene = new Scene();
	RenderPipeline* pipeline = RenderPipelineBuilder::getRenderPipelineConfigForward(uiController, scene);
	pipeline->render();
}

void ForwardRenderPipelineTest::then(MockGraphicDevice* device){
	this->assertDeviceAccess(device, "generateFrameBuffer", 1);
	this->assertDeviceAccess(device, "attachDepthBufferToFrameBufferAsTexture", 1);
	this->assertDeviceAccess(device, "setViewport", 1);
	this->assertDeviceAccess(device, "bindFrameBuffer", 1);
	this->assertDeviceAccess(device, "clearColorAndDepthBuffer", 1);
	this->assertDeviceAccess(device, "generateVboBuffer", 6);
	this->assertDeviceAccess(device, "generateVaoBuffer", 2);
}