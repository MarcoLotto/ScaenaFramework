#include "TwoEqualMeshesLoadTest.h"
#include "MeshFactory.h"

void TwoEqualMeshesLoadTest::when(){
	this->mesh = MeshFactory::getInstance()->createMeshFromFile("assets/model1.smf");
	this->mesh = MeshFactory::getInstance()->createMeshFromFile("assets/model1.smf");
}

void TwoEqualMeshesLoadTest::then(MockGraphicDevice* device){
	// TODO: Quedo vivo el mesh en la instacia del singleton, ver de borrarlo
	this->assertDeviceAccess(device, "generateMipMapFor2DTexture", 0);
	this->assertDeviceAccess(device, "generateTexture", 0);
	this->assertDeviceAccess(device, "setTextureParameterFor2DTexture", 0);
	this->assertDeviceAccess(device, "bindVaoBuffer", 0);
	this->assertDeviceAccess(device, "fillVboWithData", 0);
	this->assertDeviceAccess(device, "generateVaoBuffer", 0);
	this->assertDeviceAccess(device, "generateVboBuffer", 0);
	this->assertTrue("vertex buffer", this->mesh->getChilds()->at(0)->getMeshBuffers()->getVertexBuffer() != NULL);
}