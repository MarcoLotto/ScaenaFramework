#pragma once
#include "Test.h"

class BloomRenderPipelineTest : public Test {

public:
	virtual void when();
	virtual void then(MockGraphicDevice* device);

	virtual string getName(){ return "BloomRenderPipelineTest"; }
};