#include "TestConfiguration.h"

#include "ForwardRenderPipelineTest.h"
#include "BloomRenderPipelineTest.h"
#include "BlurRenderPipelineTest.h"
#include "UIRenderPipelineTest.h"
#include "DeferredRenderPipelineTest.h"
#include "DepthOfFieldRenderPipelineTest.h"

void TestConfiguration::configureTests(){
	this->tests.push_back(new ForwardRenderPipelineTest());
	this->tests.push_back(new BloomRenderPipelineTest());
	this->tests.push_back(new BlurRenderPipelineTest());
	this->tests.push_back(new UIRenderPipelineTest());
	this->tests.push_back(new DeferredRenderPipelineTest());
	this->tests.push_back(new DepthOfFieldRenderPipelineTest());
}