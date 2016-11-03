#include "MeshLoadTest.h"
#include "MeshFactory.h"

void MeshLoadTest::when(){
	this->mesh = MeshFactory::getInstance()->createMeshFromFile("assets/model1.smf");
}

void MeshLoadTest::then(MockGraphicDevice* device){
	this->assertDeviceAccess(device, "generateMipMapFor2DTexture", 1);
	this->assertDeviceAccess(device, "generateTexture", 1);
	this->assertDeviceAccess(device, "setTextureParameterFor2DTexture", 3);
	this->assertDeviceAccess(device, "bindVaoBuffer", 12);
	this->assertDeviceAccess(device, "fillVboWithData", 6);
	this->assertDeviceAccess(device, "generateVaoBuffer", 1);
	this->assertDeviceAccess(device, "generateVboBuffer", 6);
	this->assertTrue("vertex buffer", this->mesh->getChilds()->at(0)->getMeshBuffers()->getVertexBuffer() != NULL);
}