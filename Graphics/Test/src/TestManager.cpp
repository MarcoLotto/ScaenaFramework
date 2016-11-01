#include "TestManager.h"
#include "AssertionException.h"
#include "FrameBufferManager.h"

TestManager::TestManager() { 
	this->mockDevice = new MockGraphicDevice();
	GraphicDevice::bindCustomApi(this->mockDevice);
}

void TestManager::runTests()
{
	TestConfiguration* testConfiguration = new TestConfiguration();
	list<Test*> testsToRun = testConfiguration->getTests();
	int failed = 0;
	int passed = 0;

	// Corremos cada uno de los tests
	std::list<Test*>::iterator it = testsToRun.begin();
	while(it != testsToRun.end()){
		Test* test = *it;
		try {
			test->when();
			test->then(this->mockDevice);
			Logger::getInstance()->logInfo(new Log("PASSED: " + test->getName()));

			this->resetData();
			passed++;
		}
		catch(...){
			Logger::getInstance()->logInfo(new Log("FAILED: " + test->getName()));
			failed++;
		}
		++it;
	}
	Logger::getInstance()->logInfo(new Log("TESTING FINISHED - PASSED: " + StringUtils::toString(passed) + " - FAILED: " + StringUtils::toString(failed)));
}

void TestManager::resetData() {
	this->mockDevice->resetDeviceInfo();
	FrameBufferManager::getInstance()->clearAllFrameBuffers();
	//_sleep(6000);
}