#pragma once
#include "TestConfiguration.h"
#include "MockGraphicDevice.h"

class TestManager {

private:
	MockGraphicDevice* mockDevice;
	void resetData();

public:
	TestManager();

	// Corre toda la suite de tests
	void runTests();
};