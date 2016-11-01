#pragma once
#include "Test.h"

class BlurRenderPipelineTest : public Test {

public:
	virtual void when();
	virtual void then(MockGraphicDevice* device);

	virtual string getName(){ return "BlurRenderPipelineTest"; }
};