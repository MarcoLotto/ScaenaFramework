#pragma once
#include "Test.h"
#include "Object.h"

class TwoEqualObjectsLoadTest : public Test {

private:
	Object* object;

public:
	virtual void when();
	virtual void then(MockGraphicDevice* device);

	virtual string getName(){ return "TwoEqualObjectsLoadTest"; }
};