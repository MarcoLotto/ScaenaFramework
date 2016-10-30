#include "TestConfiguration.h"

#include "RenderPipelineTest.h"

void TestConfiguration::configureTests(){
	this->tests.push_back(new RenderPipelineTest());
}