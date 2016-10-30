#pragma once
#include "TestConfiguration.h"
#include "MockGraphicDevice.h"

class TestManager {

private:
	MockGraphicDevice* mockDevice;

public:
	TestManager();

	// Corre toda la suite de tests
	void runTests();
};