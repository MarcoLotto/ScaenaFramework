#include "ObjectLoadTest.h"
#include "ObjectLoader.h"

void ObjectLoadTest::when(){
	this->object = ObjectLoader::loadObjectFromFile("assets/object1.xml");
}

void ObjectLoadTest::then(MockGraphicDevice* device){
	this->assertDeviceAccess(device, "bindVaoBuffer", 12);
	this->assertDeviceAccess(device, "fillVboWithData", 6);
	this->assertDeviceAccess(device, "generateVaoBuffer", 1);
	this->assertDeviceAccess(device, "generateVboBuffer", 6);
	this->assertTrue("vertex buffer", this->object->getMesh()->getChilds()->at(0)->getMeshBuffers()->getVertexBuffer() != NULL);
}