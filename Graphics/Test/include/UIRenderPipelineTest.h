#pragma once
#include "Test.h"

class UIRenderPipelineTest : public Test {

public:
	virtual void when();
	virtual void then(MockGraphicDevice* device);

	virtual string getName(){ return "UIRenderPipelineTest"; }
};