#pragma once
#include "Test.h"
#include "Texture.h"

class OneTextureTest : public Test {

private:
	Texture* texture;

public:
	virtual void when();
	virtual void then(MockGraphicDevice* device);

	virtual string getName(){ return "OneTextureTest"; }
};