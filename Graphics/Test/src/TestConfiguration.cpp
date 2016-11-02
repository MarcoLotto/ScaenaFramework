#include "TestConfiguration.h"

#include "ForwardRenderPipelineTest.h"
#include "BloomRenderPipelineTest.h"
#include "BlurRenderPipelineTest.h"
#include "UIRenderPipelineTest.h"
#include "DeferredRenderPipelineTest.h"
#include "DepthOfFieldRenderPipelineTest.h"
#include "OneTextureTest.h"
#include "TwoDifferentTexturesTest.h"
#include "TwoEqualTexturesTest.h"

void TestConfiguration::configureTests(){
	this->tests.push_back(new ForwardRenderPipelineTest());
	this->tests.push_back(new BloomRenderPipelineTest());
	this->tests.push_back(new BlurRenderPipelineTest());
	this->tests.push_back(new UIRenderPipelineTest());
	this->tests.push_back(new DeferredRenderPipelineTest());
	this->tests.push_back(new DepthOfFieldRenderPipelineTest());
	this->tests.push_back(new TwoDifferentTexturesTest());
	this->tests.push_back(new TwoEqualTexturesTest());
	this->tests.push_back(new OneTextureTest());
}