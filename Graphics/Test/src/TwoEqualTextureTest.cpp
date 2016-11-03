#include "TwoEqualTexturesTest.h"
#include "FileTexture.h"

// La prueba es que al cargar dos imagenes iguales solo se cargue una vez (por lo que tendra las mismas llamadas a la API que una sola)
void TwoEqualTexturesTest::when(){
	this->texture1 = new FileTexture("assets/img3.png");
	this->texture2 = new FileTexture("assets/img3.png");
}

void TwoEqualTexturesTest::then(MockGraphicDevice* device){
	this->assertDeviceAccess(device, "generateMipMapFor2DTexture", 1);
	this->assertDeviceAccess(device, "generateTexture", 1);
	this->assertDeviceAccess(device, "setTextureParameterFor2DTexture", 3);
	this->assertTrue("texture 1 valid", this->texture1->isTextureValid());
	this->assertTrue("texture 2 valid", this->texture2->isTextureValid());
	
	delete this->texture1;
	delete this->texture2;
}