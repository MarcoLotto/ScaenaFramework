#include "TwoDifferentTexturesTest.h"
#include "FileTexture.h"

void TwoDifferentTexturesTest::when(){
	this->texture1 = new FileTexture("assets/img1.png");
	this->texture2 = new FileTexture("assets/img2.png");
}

void TwoDifferentTexturesTest::then(MockGraphicDevice* device){
	this->assertDeviceAccess(device, "generateMipMapFor2DTexture", 2);
	this->assertDeviceAccess(device, "generateTexture", 2);
	this->assertDeviceAccess(device, "setTextureParameterFor2DTexture", 6);
	this->assertTrue("texture 1 valid", this->texture1->isTextureValid());
	this->assertTrue("texture 2 valid", this->texture2->isTextureValid());

	delete this->texture1;
	delete this->texture2;
}