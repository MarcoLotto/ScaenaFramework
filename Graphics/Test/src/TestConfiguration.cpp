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
#include "MeshLoadTest.h"
#include "TwoEqualMeshesLoadTest.h"
#include "ObjectLoadTest.h"
#include "TwoEqualObjectsLoadTest.h"
#include "SceneLoadTest.h"
#include "SceneLightsLoadTest.h"

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
	this->tests.push_back(new MeshLoadTest());
	this->tests.push_back(new TwoEqualMeshesLoadTest());
	this->tests.push_back(new ObjectLoadTest());
	this->tests.push_back(new TwoEqualObjectsLoadTest());
	this->tests.push_back(new SceneLoadTest());
	this->tests.push_back(new SceneLightsLoadTest());
}