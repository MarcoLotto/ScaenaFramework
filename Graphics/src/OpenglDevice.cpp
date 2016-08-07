/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include <GL/glew.h>
#include <glm/glm.hpp> 

#include "OpenglDevice.h"
#include "GraphicInitializationException.h"
#include "ApiVariableTypeDoesNotExistException.h"
#include "OpenglConstantCatalog.h"
#include "Logger.h"
#include "VideoConfiguration.h"
#include "StringUtils.h"

OpenglDevice::OpenglDevice(){
	this->constantCatalog = new OpenglConstantCatalog();
}

void OpenglDevice::setViewport(unsigned int x, unsigned int y, int width, int height){
	glViewport(x, y, width, height);
	this->evaluateErrorsAndLogOnlyInDebugMode("setViewport");
}
void OpenglDevice::enableMode(unsigned int mode){
	glEnable(mode);
	this->evaluateErrorsAndLogOnlyInDebugMode("enableMode");
}
void OpenglDevice::disableMode(unsigned int mode){
	glDisable(mode);
	this->evaluateErrorsAndLogOnlyInDebugMode("disableMode");
}

void OpenglDevice::frontFaceSelector(unsigned int faceFlag){
	glFrontFace(faceFlag);
	this->evaluateErrorsAndLogOnlyInDebugMode("frontFaceSelector");
}

// Limpieza del buffer activo
void OpenglDevice::clearColorBuffer(){ 
	glClear(GL_COLOR_BUFFER_BIT); 
	this->evaluateErrorsAndLogOnlyInDebugMode("clearColorBuffer");
}
void OpenglDevice::clearDepthBuffer(){ 
	glClear(GL_DEPTH_BUFFER_BIT); 
	this->evaluateErrorsAndLogOnlyInDebugMode("clearDepthBuffer");
}
void OpenglDevice::clearColorAndDepthBuffer(){ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	this->evaluateErrorsAndLogOnlyInDebugMode("clearColorAndDepthBuffer");
}

// Setea el color con el que limpiar
void OpenglDevice::clearColor(float r, float g, float b, float a){
	glClearColor(r, g, b, a);
	this->evaluateErrorsAndLogOnlyInDebugMode("clearColor");
}

// Para definir como se procesa el blend
void OpenglDevice::blendFunction(unsigned int sFactor, unsigned int dFactor){
	glBlendFunc (sFactor, dFactor);
	this->evaluateErrorsAndLogOnlyInDebugMode("defineBlendFunction");
}

unsigned int OpenglDevice::generateFrameBuffer(){
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	this->evaluateErrorsAndLog("generateFrameBuffer");
	return fbo;
}

unsigned int OpenglDevice::generateDepthBuffer(int width, int height){
	unsigned int depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	this->evaluateErrorsAndLog("generateDepthBuffer(1)");
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	this->evaluateErrorsAndLog("generateDepthBuffer(2)");
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	this->evaluateErrorsAndLog("generateDepthBuffer(3)");
	return depthBuffer;
}

void OpenglDevice::attachDepthBufferToFrameBufferAsRenderBuffer(unsigned int depthBuffer, unsigned int frameBuffer){
	this->bindFrameBuffer(frameBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	this->evaluateErrorsAndLogOnlyInDebugMode("attachDepthBufferToFrameBufferAsRenderBuffer");
}

void OpenglDevice::attachDepthBufferToFrameBufferAsTexture(unsigned int depthBuffer, unsigned int frameBuffer){
	this->bindFrameBuffer(frameBuffer);
	if(depthBuffer != 0){		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
		this->evaluateErrorsAndLogOnlyInDebugMode("attachDepthBufferToFrameBufferAsTexture");
	}
}

void OpenglDevice::bindTextureToFrameBuffer(ApiTexture* texture, unsigned int frameBufferHandler, unsigned int attachmentPosition){
	this->bindFrameBuffer(frameBufferHandler);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentPosition,
	GL_TEXTURE_2D, texture->getTextureId(), 0);
	this->evaluateErrorsAndLogOnlyInDebugMode("bindTextureToFrameBuffer");
}

void OpenglDevice::drawFrameBuffer(unsigned int frameBufferHandler, unsigned int colorAttachmentsCount){
	GLenum* drawBuffers = new GLenum[colorAttachmentsCount + 1]();
	for(unsigned int i = 0; i < colorAttachmentsCount; i++){
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers(colorAttachmentsCount, drawBuffers);
	this->evaluateErrorsAndLogOnlyInDebugMode("drawFrameBuffer");
	delete drawBuffers;
}

void OpenglDevice::drawFrameBufferToBackBuffer(){
	GLenum drawBuffers[] = { GL_NONE, GL_BACK_LEFT };
	glDrawBuffers(2, drawBuffers);
	this->evaluateErrorsAndLogOnlyInDebugMode("drawFrameBufferToBackBuffer");
}

void OpenglDevice::bindFrameBuffer(unsigned int frameBufferHandler){
    if(frameBufferHandler == 0)
        frameBufferHandler = GraphicDevice::defaultFrameBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferHandler);
	this->evaluateErrorsAndLogOnlyInDebugMode("bindFrameBuffer");
	
	// Verifico el estado del Frame buffer
	int result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(result != GL_FRAMEBUFFER_COMPLETE && result != GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT){
		//Estoy bindeando un framebuffer que no esta completo ni vacio
		string message = "OpenGl Warning: FBO incomplete in OpenglDevice#bindFrameBuffer : glCheckFramebufferStatus result code: " + StringUtils::toString(result); 
		Logger::getInstance()->logError(new Log(message));
	}
}

unsigned int OpenglDevice::getCurrentBindedFrameBuffer(){
	int bindedFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bindedFbo);
	this->evaluateErrorsAndLogOnlyInDebugMode("getCurrentBindedFrameBuffer");
	return (unsigned int) bindedFbo;
}


unsigned char* OpenglDevice::getApiVersion(){
	return (unsigned char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
}

unsigned int OpenglDevice::generateTexture(vec2 size, unsigned int internalFormat, unsigned int format, void* pixelData){
	unsigned int textureId;
	glGenTextures(1, &textureId);	
	this->evaluateErrorsAndLog("generateTexture(1)");
	glBindTexture(GL_TEXTURE_2D, textureId);	
	this->evaluateErrorsAndLog("generateTexture(2)");
	
	// Elijo el tipo en base al formato interno
	unsigned int type = GL_UNSIGNED_BYTE;
	if(internalFormat == GL_DEPTH_COMPONENT24 || internalFormat == GL_DEPTH_COMPONENT16){
		type = GL_UNSIGNED_INT;
	}
	else if(internalFormat == GL_RGB32F || internalFormat == GL_RGB16F || internalFormat == GL_RG32F
			|| internalFormat == GL_RG16F  || internalFormat == GL_R32F || internalFormat == GL_R16F
			|| internalFormat == GL_RGBA32F || internalFormat == GL_RGBA16F){
		type = GL_FLOAT;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, pixelData);
	this->evaluateErrorsAndLog("generateTexture(3)");
	return textureId;
}

void OpenglDevice::setActiveTexture(unsigned int textureUnit){
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	this->evaluateErrorsAndLogOnlyInDebugMode("setActiveTexture");
}

void OpenglDevice::bindTexture(unsigned int textureId){
	glBindTexture(GL_TEXTURE_2D, textureId);	
	this->evaluateErrorsAndLogOnlyInDebugMode("bindTexture");
}
void OpenglDevice::setTextureParameterFor2DTexture(unsigned int parameterType, unsigned int value){
	glTexParameteri(GL_TEXTURE_2D, parameterType, value);
	this->evaluateErrorsAndLogOnlyInDebugMode("setTextureParameterFor2DTexture");
}
void OpenglDevice::setTextureParameterFor2DTexture(unsigned int parameterType, float* value){
	glTexParameterfv(GL_TEXTURE_2D, parameterType, value);
	this->evaluateErrorsAndLogOnlyInDebugMode("setTextureParameterFor2DTexture");
}
void OpenglDevice::deleteTexture(unsigned int textureId){
	glDeleteTextures(1, &textureId);	
	this->evaluateErrorsAndLog("deleteTexture");
}
void OpenglDevice::deleteFrameBuffer(unsigned int frameBuffer){
	glDeleteFramebuffers(1, &frameBuffer);
	this->evaluateErrorsAndLog("deleteFrameBuffer");
}
void OpenglDevice::deleteDepthBuffer(unsigned int depthBuffer){
	glDeleteRenderbuffers(1, &depthBuffer);
	this->evaluateErrorsAndLog("deleteDepthBuffer");
}
void OpenglDevice::generateMipMapFor2DTexture(int width, int height){
	glGenerateMipmap(GL_TEXTURE_2D);
	this->evaluateErrorsAndLog("generateMipMapFor2DTexture");
}

void OpenglDevice::initiate(){
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		this->evaluateErrorsAndLog("initiate");
		throw new GraphicInitializationException("Fallo al iniciar OpenGL");
	}
}

void OpenglDevice::destroy(){
	// TODO
}

void OpenglDevice::drawMesh(Mesh* mesh){
	this->drawMesh(mesh->getMeshBuffers());	
}
void OpenglDevice::transformUsingFeedback(VertexArrayObject* vao, unsigned int vertexCount, TransformFeedbackObject* tfo){
	// TODO: Chicos, no hagan esto en casa, es un asco, refactor urgente
	glEnable(GL_RASTERIZER_DISCARD);	
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfo->getHandler());
	this->evaluateErrorsAndLogOnlyInDebugMode("transformMeshUsingFeedback(1)");
	glBeginTransformFeedback(GL_TRIANGLES);
	glBindVertexArray( vao->getHandler());
	glDrawArrays( GL_TRIANGLES, 0, vertexCount);	
	glEndTransformFeedback();
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDisable(GL_RASTERIZER_DISCARD);
}
void OpenglDevice::drawMesh(MeshBuffers* meshBuffers){
	glBindVertexArray( meshBuffers->getVaoHandle());
	this->evaluateErrorsAndLogOnlyInDebugMode("drawMesh(1)");
	glDrawElements(GL_TRIANGLES, meshBuffers->getIndexBufferSize(), GL_UNSIGNED_INT, (GLvoid*)0);
	this->evaluateErrorsAndLogOnlyInDebugMode("drawMesh(2)");
}
void OpenglDevice::drawUsingTransformFeedback(VertexArrayObject* vao, TransformFeedbackObject* tfo){
	glBindVertexArray( vao->getHandler());
	this->evaluateErrorsAndLogOnlyInDebugMode("drawMeshWithTransformFeedback(1)");
	glDrawTransformFeedback(GL_TRIANGLES, tfo->getHandler());
	this->evaluateErrorsAndLogOnlyInDebugMode("drawMeshWithTransformFeedback(2)");
}
void OpenglDevice::drawQuads(unsigned int vaoHandle, unsigned int vertexCount){
	glBindVertexArray( vaoHandle );
	this->evaluateErrorsAndLogOnlyInDebugMode("drawQuads(1)");
	glDrawArrays( GL_QUADS, 0, vertexCount);
	this->evaluateErrorsAndLogOnlyInDebugMode("drawQuads(2)");
}
void OpenglDevice::drawTriangleStrip(unsigned int vaoHandle, unsigned int* indexBuffer, unsigned int vertexCount){
	glBindVertexArray( vaoHandle );
	this->evaluateErrorsAndLogOnlyInDebugMode("drawTriangleStrip(1)");
	glDrawElements(GL_TRIANGLE_STRIP, vertexCount, GL_UNSIGNED_INT, indexBuffer);
	this->evaluateErrorsAndLogOnlyInDebugMode("drawTriangleStrip(2)");
}
void OpenglDevice::drawLine(unsigned int vaoHandle){
	glBindVertexArray(vaoHandle);
	this->evaluateErrorsAndLogOnlyInDebugMode("drawLine(1)");
	glDrawArrays( GL_LINES, 0, 2);		
	this->evaluateErrorsAndLogOnlyInDebugMode("drawLine(2)");
}
void OpenglDevice::drawBuffer(unsigned int colorAttachmentNumber){
	glDrawBuffer( GL_COLOR_ATTACHMENT0 + colorAttachmentNumber );		
	this->evaluateErrorsAndLogOnlyInDebugMode("drawBuffer");
}

unsigned int OpenglDevice::generateVaoBuffer(){
	unsigned int handle;
	glGenVertexArrays( 1, &handle);
	this->evaluateErrorsAndLog("generateVaoBuffer(1)");
	return handle;
}
unsigned int OpenglDevice::generateVboBuffer(){
	unsigned int handle;
	glGenBuffers(1, &handle);
	this->evaluateErrorsAndLog("generateVboBuffer(1)");
	return handle;
}
unsigned int OpenglDevice::generateTransformFeedback(){
	unsigned int handle;
	glGenTransformFeedbacks(1, &handle);
	this->evaluateErrorsAndLog("generateTransformFeedback(1)");	
	return handle;
}

void OpenglDevice::deleteTransformFeedback(unsigned int feedbackHandle){
	glDeleteTransformFeedbacks(1, &feedbackHandle);
}
void OpenglDevice::deleteVboBuffer(unsigned int vboHandle){
	glDeleteBuffers(1, &vboHandle);
}
void OpenglDevice::deleteVaoBuffer(unsigned int vaoHandle){
	glDeleteVertexArrays(1, &vaoHandle);
}

unsigned int OpenglDevice::attachVboToTransformFeedback(unsigned int feedbackHandle, unsigned int vboHandle, unsigned int attachIndex){	
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedbackHandle);
	this->evaluateErrorsAndLog("attachVboToTransformFeedback(1)");
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, attachIndex, vboHandle);
	this->evaluateErrorsAndLog("attachVboToTransformFeedback(2)");
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	return feedbackHandle;
}

void OpenglDevice::bindVaoBuffer(unsigned int vaoHandle){	
	glBindVertexArray( vaoHandle );
	this->evaluateErrorsAndLogOnlyInDebugMode("bindVaoBuffer(1)");
}


void OpenglDevice::attachVboToVao(VertexBufferObject* vbo, unsigned int attachIndex){
	glBindBuffer( GL_ARRAY_BUFFER, vbo->getHandler() );
	glEnableVertexAttribArray(attachIndex);	
	this->evaluateErrorsAndLog("attachVboToVao(1)");
	glVertexAttribPointer( attachIndex, vbo->getComponentCount(), vbo->getDataType(), GL_FALSE, 0, (GLubyte*)NULL);
	this->evaluateErrorsAndLog("attachVboToVao(2)");	
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

// Carga un vertex buffer object en memoria de video
void OpenglDevice::fillVboWithData(GraphicArrayContainer* container){
	glBindBuffer( GL_ARRAY_BUFFER, container->getVboHandle() );
	this->evaluateErrorsAndLog("fillVboWithData(1)");
	unsigned int drawMethod = container->getDrawMethod(); 
	unsigned int dataSize = this->getArrayContainerDataTypeSize(container->getDataType());
	glBufferData( GL_ARRAY_BUFFER, container->getBufferSize() * dataSize, container->getBuffer(), drawMethod );	
	this->evaluateErrorsAndLog("fillVboWithData(2)");

	unsigned int attachIndex = container->getVao()->getAttachementsCount();
	glEnableVertexAttribArray(attachIndex);	
	this->evaluateErrorsAndLog("fillVboWithData(3)");
	glVertexAttribPointer( attachIndex, container->getComponentCount(), container->getDataType(), GL_FALSE, 0, (GLubyte*)NULL);
	this->evaluateErrorsAndLog("fillVboWithData(4)");
	container->getVao()->addAttachementCount();	
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void OpenglDevice::fillVboWithData(GraphicIndexArrayContainer* container){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, container->getVboHandle());
	this->evaluateErrorsAndLog("fillVboWithData(5)");
	unsigned int drawMethod = container->getDrawMethod(); 
	unsigned int dataSize = this->getArrayContainerDataTypeSize(container->getDataType());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, container->getBufferSize()*dataSize, container->getBuffer(), drawMethod);
	this->evaluateErrorsAndLog("fillVboWithData(6)");
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void OpenglDevice::modifyGeometryArray(GraphicArrayContainer* container, unsigned int vboHandle){
	glBindBuffer( GL_ARRAY_BUFFER, vboHandle );
	this->evaluateErrorsAndLog("modifyGeometryArray(1)");
	unsigned int drawMethod = container->getDrawMethod();
	unsigned int dataSize = this->getArrayContainerDataTypeSize(container->getDataType());
	glBufferData( GL_ARRAY_BUFFER, container->getBufferSize() * dataSize, container->getBuffer(), drawMethod );
	this->evaluateErrorsAndLog("modifyGeometryArray(2)");
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

unsigned int OpenglDevice::getArrayContainerDataTypeSize(unsigned int dataTypeInContainer){
	if(dataTypeInContainer == this->constantCatalog->getFloatTypeFlag())
		return sizeof(float);
	if(dataTypeInContainer == this->constantCatalog->getUnsignedIntTypeFlag())
		return sizeof(unsigned int);
    if(dataTypeInContainer == this->constantCatalog->getUnsignedShortTypeFlag())
        return sizeof(unsigned short);
	//Agregar otros tipos aca
	
	//El tipo que buscan no esta declarado o no existe en el api
	throw new ApiVariableTypeDoesNotExistException();
}

void OpenglDevice::evaluateErrorsAndLog(const char* location){
	unsigned int glErr = glGetError();
    if (glErr != GL_NO_ERROR)
	{
		char* errorDescription = (char*) gluErrorString(glErr);
		string message = "";
		if(errorDescription != NULL){
			message = "OpenGl error in OpenglDevice#" + string(location) + " : " + (char*) gluErrorString(glErr); 
		}
		else{
			//No tengo la descripcion del error, logueo el codigo
			message = "OpenGl error in OpenglDevice#" + string(location) + " : " + "OpengGL error code: " + StringUtils::toString( glErr); 
		}
		Logger::getInstance()->logError(new Log(message));
    }
}

void OpenglDevice::evaluateErrorsAndLogOnlyInDebugMode(const char* location){
	if(VideoConfiguration::getInstance()->isDebugMode()){
		this->evaluateErrorsAndLog(location);
	}
}

// Para soporte de queries
unsigned int OpenglDevice::generateQuery(){
	unsigned int query;
	glGenQueries(1, &query);
	this->evaluateErrorsAndLog("generateQuery(1)");
	return query;
}
void OpenglDevice::beginQuery(unsigned int queryHandler, unsigned int queryType){
	glBeginQuery(queryType, queryHandler);
	this->evaluateErrorsAndLogOnlyInDebugMode("beginQuery(1)");
}
void OpenglDevice::endQuery(unsigned int queryType){
	glEndQuery(queryType);
	this->evaluateErrorsAndLogOnlyInDebugMode("endQuery(1)");
}
unsigned int OpenglDevice::getQueryResultAsUInt(unsigned int queryHandler){
	GLint done = 0;
	while (!done) {
		glGetQueryObjectiv(queryHandler, GL_QUERY_RESULT_AVAILABLE, &done);
		this->evaluateErrorsAndLogOnlyInDebugMode("getQueryResultAsUInt(1)");
	}
	unsigned int queryResult;
	glGetQueryObjectuiv(queryHandler, GL_QUERY_RESULT, &queryResult);
	this->evaluateErrorsAndLogOnlyInDebugMode("getQueryResultAsUInt(2)");
	return queryResult;
}

void OpenglDevice::deleteQuery(unsigned int queryHandler){
	glDeleteQueries(1, &queryHandler);
	this->evaluateErrorsAndLog("deleteQuery(1)");
}
