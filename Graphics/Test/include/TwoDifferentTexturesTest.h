#pragma once
#include "Test.h"
#include "Texture.h"

class TwoDifferentTexturesTest : public Test {

private:
	Texture* texture1;
	Texture* texture2;

public:
	virtual void when();
	virtual void then(MockGraphicDevice* device);

	virtual string getName(){ return "TwoDifferentTexturesTest"; }
};