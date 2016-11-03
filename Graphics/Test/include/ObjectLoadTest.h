#pragma once
#include "Test.h"
#include "Object.h"

class ObjectLoadTest : public Test {

private:
	Object* object;

public:
	virtual void when();
	virtual void then(MockGraphicDevice* device);

	virtual string getName(){ return "ObjectLoadTest"; }
};