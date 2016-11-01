/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/
#include "MockGraphicDevice.h"
#include "OpenglConstantCatalog.h"
#include "Logger.h"

void MockGraphicDevice::addMethodInfo(string methodName){
	map<string, int>::iterator it = this->methodsInfo.find(methodName);
	if(it != this->methodsInfo.end())
	{
	   this->methodsInfo[methodName] += 1;
	}
	else{
		this->methodsInfo[methodName] = 1;
	}
}

int MockGraphicDevice::getMethodCalls(string methodName){
	map<string, int>::iterator it = this->methodsInfo.find(methodName);
	if(it != this->methodsInfo.end())
	{
		return it->second;
	}
	return 0;
}

MockGraphicDevice::MockGraphicDevice(){
	this->addMethodInfo("MockGraphicDevice");
	this->constantCatalog = new OpenglConstantCatalog();
	this->returnValue = 1;
}

int MockGraphicDevice::getNextReturnValue() {
	return this->returnValue++;
}

void MockGraphicDevice::setViewport(unsigned int x, unsigned int y, int width, int height){
	this->addMethodInfo("setViewport");
}
void MockGraphicDevice::enableMode(unsigned int mode){
	this->addMethodInfo("enableMode");
}
void MockGraphicDevice::disableMode(unsigned int mode){
	this->addMethodInfo("disableMode");
}

void MockGraphicDevice::frontFaceSelector(unsigned int faceFlag){
	this->addMethodInfo("frontFaceSelector");
}

// Limpieza del buffer activo
void MockGraphicDevice::clearColorBuffer(){ 
	this->addMethodInfo("clearColorBuffer");
}
void MockGraphicDevice::clearDepthBuffer(){ 
	this->addMethodInfo("clearDepthBuffer");
}
void MockGraphicDevice::clearColorAndDepthBuffer(){ 
	this->addMethodInfo("clearColorAndDepthBuffer");
}

// Setea el color con el que limpiar
void MockGraphicDevice::clearColor(float r, float g, float b, float a){
	this->addMethodInfo("clearColor");
}

// Para definir como se procesa el blend
void MockGraphicDevice::blendFunction(unsigned int sFactor, unsigned int dFactor){
	this->addMethodInfo("blendFuction");
}

unsigned int MockGraphicDevice::generateFrameBuffer(){
	this->addMethodInfo("generateFrameBuffer");
	return this->getNextReturnValue();
}

unsigned int MockGraphicDevice::generateDepthBuffer(int width, int height){
	this->addMethodInfo("generateDepthBuffer");
	return this->getNextReturnValue();
}

void MockGraphicDevice::attachDepthBufferToFrameBufferAsRenderBuffer(unsigned int depthBuffer, unsigned int frameBuffer){
	this->addMethodInfo("attachDepthBufferToFrameBufferAsRenderBuffer");
}

void MockGraphicDevice::attachDepthBufferToFrameBufferAsTexture(unsigned int depthBuffer, unsigned int frameBuffer){
	this->addMethodInfo("attachDepthBufferToFrameBufferAsTexture");
}

void MockGraphicDevice::bindTextureToFrameBuffer(ApiTexture* texture, unsigned int frameBufferHandler, unsigned int attachmentPosition){
	this->addMethodInfo("bindTextureToFrameBuffer");
}

void MockGraphicDevice::drawFrameBuffer(unsigned int frameBufferHandler, unsigned int colorAttachmentsCount){
	this->addMethodInfo("drawFrameBuffer");
}

void MockGraphicDevice::drawFrameBufferToBackBuffer(){
	this->addMethodInfo("drawFrameBufferToBackBuffer");
}

void MockGraphicDevice::bindFrameBuffer(unsigned int frameBufferHandler){
    this->addMethodInfo("bindFrameBuffer");
}

unsigned int MockGraphicDevice::getCurrentBindedFrameBuffer(){
	this->addMethodInfo("getCurrentBindedFrameBuffer");
	return this->getNextReturnValue();
}


unsigned char* MockGraphicDevice::getApiVersion(){
	this->addMethodInfo("getApiVersion");
	unsigned char* version = new unsigned char[1];
	version[0] = '4';
	return version;
}

unsigned int MockGraphicDevice::generateTexture(vec2 size, unsigned int internalFormat, unsigned int format, void* pixelData){
	this->addMethodInfo("generateTexture");
	return this->getNextReturnValue();
}

void MockGraphicDevice::setActiveTexture(unsigned int textureUnit){
	this->addMethodInfo("setActiveTexture");
}

void MockGraphicDevice::bindTexture(unsigned int textureId){
	this->addMethodInfo("bindTexture");
}
void MockGraphicDevice::setTextureParameterFor2DTexture(unsigned int parameterType, unsigned int value){
	this->addMethodInfo("setTextureParameterFor2DTexture");
}
void MockGraphicDevice::setTextureParameterFor2DTexture(unsigned int parameterType, float* value){
	this->addMethodInfo("setTextureParameterFor2DTexture");
}
void MockGraphicDevice::deleteTexture(unsigned int textureId){
	this->addMethodInfo("deleteTexture");
}
void MockGraphicDevice::deleteFrameBuffer(unsigned int frameBuffer){
	this->addMethodInfo("deleteFrameBuffer");
}
void MockGraphicDevice::deleteDepthBuffer(unsigned int depthBuffer){
	this->addMethodInfo("deleteDepthBuffer");
}
void MockGraphicDevice::generateMipMapFor2DTexture(int width, int height){
	this->addMethodInfo("generateMipMapFor2DTexture");
}

void MockGraphicDevice::initiate(){
	this->addMethodInfo("initiate");
}

void MockGraphicDevice::destroy(){
	this->addMethodInfo("destroy");
}

void MockGraphicDevice::drawMesh(Mesh* mesh){
	this->addMethodInfo("drawMesh");
}
void MockGraphicDevice::transformUsingFeedback(VertexArrayObject* vao, unsigned int vertexCount, TransformFeedbackObject* tfo){
	this->addMethodInfo("transformUsingFeedback");
}
void MockGraphicDevice::drawMesh(MeshBuffers* meshBuffers){
	this->addMethodInfo("drawMesh");
}
void MockGraphicDevice::drawUsingTransformFeedback(VertexArrayObject* vao, TransformFeedbackObject* tfo){
	this->addMethodInfo("drawUsingTransformFeedback");
}
void MockGraphicDevice::drawQuads(unsigned int vaoHandle, unsigned int vertexCount){
	this->addMethodInfo("drawQuads");
}
void MockGraphicDevice::drawTriangleStrip(unsigned int vaoHandle, unsigned int* indexBuffer, unsigned int vertexCount){
	this->addMethodInfo("drawTriangleStrip");
}
void MockGraphicDevice::drawLine(unsigned int vaoHandle){
	this->addMethodInfo("drawLine");
}
void MockGraphicDevice::drawBuffer(unsigned int colorAttachmentNumber){
	this->addMethodInfo("drawBuffer");
}

unsigned int MockGraphicDevice::generateVaoBuffer(){
	this->addMethodInfo("generateVaoBuffer");
	return this->getNextReturnValue();
}
unsigned int MockGraphicDevice::generateVboBuffer(){
	this->addMethodInfo("generateVboBuffer");
	return this->getNextReturnValue();
}
unsigned int MockGraphicDevice::generateTransformFeedback(){
	this->addMethodInfo("generateTransformFeedback");
	return this->getNextReturnValue();
}

void MockGraphicDevice::deleteTransformFeedback(unsigned int feedbackHandle){
	this->addMethodInfo("deleteTransformFeedback");
}
void MockGraphicDevice::deleteVboBuffer(unsigned int vboHandle){
	this->addMethodInfo("deleteVboBuffer");
}
void MockGraphicDevice::deleteVaoBuffer(unsigned int vaoHandle){
	this->addMethodInfo("deleteVaoBuffer");
}

unsigned int MockGraphicDevice::attachVboToTransformFeedback(unsigned int feedbackHandle, unsigned int vboHandle, unsigned int attachIndex){	
	this->addMethodInfo("attachVboToTransformFeedback");
	return this->getNextReturnValue();
}

void MockGraphicDevice::bindVaoBuffer(unsigned int vaoHandle){	
	this->addMethodInfo("bindVaoBuffer");
}

void MockGraphicDevice::attachVboToVao(VertexBufferObject* vbo, unsigned int attachIndex){
	this->addMethodInfo("attachVboToVao");	
}

// Carga un vertex buffer object en memoria de video
void MockGraphicDevice::fillVboWithData(GraphicArrayContainer* container){
	this->addMethodInfo("fillVboWithData");
}

void MockGraphicDevice::fillVboWithData(GraphicIndexArrayContainer* container){
	this->addMethodInfo("fillVboWithData");
}

void MockGraphicDevice::modifyGeometryArray(GraphicArrayContainer* container, unsigned int vboHandle){
	this->addMethodInfo("modifyGeometryArray");
}

unsigned int MockGraphicDevice::getArrayContainerDataTypeSize(unsigned int dataTypeInContainer){
	this->addMethodInfo("getArrayContainerDataTypeSize");
	return this->getNextReturnValue();
}

void MockGraphicDevice::evaluateErrorsAndLog(const char* location){
	this->addMethodInfo("evaluateErrorsAndLog");
}

void MockGraphicDevice::evaluateErrorsAndLogOnlyInDebugMode(const char* location){
	this->addMethodInfo("evaluateErrorsAndLogOnlyInDebugMode");
}

// Para soporte de queries
unsigned int MockGraphicDevice::generateQuery(){
	this->addMethodInfo("generateQuery");
	return this->getNextReturnValue();
}
void MockGraphicDevice::beginQuery(unsigned int queryHandler, unsigned int queryType){
	this->addMethodInfo("beginQuery");
}
void MockGraphicDevice::endQuery(unsigned int queryType){
	this->addMethodInfo("endQuery");
}
unsigned int MockGraphicDevice::getQueryResultAsUInt(unsigned int queryHandler){
	this->addMethodInfo("getQueryResultAsUInt");
	return 0;
}

void MockGraphicDevice::deleteQuery(unsigned int queryHandler){
	this->addMethodInfo("deleteQuery");
}
