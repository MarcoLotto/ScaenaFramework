#pragma once
#include "Test.h"
#include "Scene.h"

class SceneLightsLoadTest : public Test {

private:
	Scene* scene;

public:
	virtual void when();
	virtual void then(MockGraphicDevice* device);

	virtual string getName(){ return "SceneLightsLoadTest"; }
};