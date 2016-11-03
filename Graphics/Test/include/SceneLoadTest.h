#pragma once
#include "Test.h"
#include "Scene.h"

class SceneLoadTest : public Test {

private:
	Scene* scene;

public:
	virtual void when();
	virtual void then(MockGraphicDevice* device);

	virtual string getName(){ return "SceneLoadTest"; }
};