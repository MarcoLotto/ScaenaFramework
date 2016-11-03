#pragma once
#include "Test.h"
#include "Mesh.h"

class TwoEqualMeshesLoadTest : public Test {

private:
	Mesh* mesh;

public:
	virtual void when();
	virtual void then(MockGraphicDevice* device);

	virtual string getName(){ return "TwoEqualMeshesLoadTest"; }
};