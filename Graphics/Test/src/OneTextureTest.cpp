#include "OneTextureTest.h"
#include "FileTexture.h"

void OneTextureTest::when(){
	this->texture = new FileTexture("assets/img1.png");
}

void OneTextureTest::then(MockGraphicDevice* device){
	this->assertDeviceAccess(device, "generateMipMapFor2DTexture", 1);
	this->assertDeviceAccess(device, "generateTexture", 1);
	this->assertDeviceAccess(device, "setTextureParameterFor2DTexture", 3);
	this->assertTrue("textureValid", this->texture->isTextureValid());

	delete this->texture;
}