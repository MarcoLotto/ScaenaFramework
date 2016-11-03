#include "TwoEqualObjectsLoadTest.h"
#include "ObjectLoader.h"

void TwoEqualObjectsLoadTest::when(){
	this->object = ObjectLoader::loadObjectFromFile("assets/object1.xml");
	this->object = ObjectLoader::loadObjectFromFile("assets/object1.xml");
}

void TwoEqualObjectsLoadTest::then(MockGraphicDevice* device){
	this->assertDeviceAccess(device, "bindVaoBuffer", 0);
	this->assertDeviceAccess(device, "fillVboWithData", 0);
	this->assertDeviceAccess(device, "generateVaoBuffer", 0);
	this->assertDeviceAccess(device, "generateVboBuffer", 0);
	this->assertTrue("vertex buffer", this->object->getMesh()->getChilds()->at(0)->getMeshBuffers()->getVertexBuffer() != NULL);
}