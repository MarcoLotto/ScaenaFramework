/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include <GLES3/gl3.h>
#include <GLES3/gl2ext.h>
#include <glm/glm.hpp> 

#include "OpenglES3Device.h"
#include "GraphicInitializationException.h"
#include "ApiVariableTypeDoesNotExistException.h"
#include "OpenglES3ConstantCatalog.h"
#include "Logger.h"
#include "VideoConfiguration.h"
#include "StringUtils.h"

OpenglES3Device::OpenglES3Device(){
	this->constantCatalog = new OpenglES3ConstantCatalog();
}

void OpenglES3Device::setViewport(unsigned int x, unsigned int y, int width, int height){
	glViewport(x, y, width, height);
	this->evaluateErrorsAndLogOnlyInDebugMode("setViewport");
}
void OpenglES3Device::enableMode(unsigned int mode){
	glEnable(mode);
	this->evaluateErrorsAndLogOnlyInDebugMode("enableMode");
}
void OpenglES3Device::disableMode(unsigned int mode){
	glDisable(mode);
	this->evaluateErrorsAndLogOnlyInDebugMode("disableMode");
}

void OpenglES3Device::frontFaceSelector(unsigned int faceFlag){
	glFrontFace(faceFlag);
	this->evaluateErrorsAndLogOnlyInDebugMode("frontFaceSelector");
}

// Limpieza del buffer activo
void OpenglES3Device::clearColorBuffer(){ 
	glClear(GL_COLOR_BUFFER_BIT); 
	this->evaluateErrorsAndLogOnlyInDebugMode("clearColorBuffer");
}
void OpenglES3Device::clearDepthBuffer(){ 
	glClear(GL_DEPTH_BUFFER_BIT); 
	this->evaluateErrorsAndLogOnlyInDebugMode("clearDepthBuffer");
}
void OpenglES3Device::clearColorAndDepthBuffer(){ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	this->evaluateErrorsAndLogOnlyInDebugMode("clearColorAndDepthBuffer");
}

// Setea el color con el que limpiar
void OpenglES3Device::clearColor(float r, float g, float b, float a){
	glClearColor(r, g, b, a);
	this->evaluateErrorsAndLogOnlyInDebugMode("clearColor");
}

// Para definir como se procesa el blend
void OpenglES3Device::blendFunction(unsigned int sFactor, unsigned int dFactor){
	glBlendFunc (sFactor, dFactor);
	this->evaluateErrorsAndLogOnlyInDebugMode("defineBlendFunction");
}

unsigned int OpenglES3Device::generateFrameBuffer(){
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	this->evaluateErrorsAndLog("generateFrameBuffer");
	return fbo;
}

unsigned int OpenglES3Device::generateDepthBuffer(int width, int height){
	unsigned int depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	this->evaluateErrorsAndLog("generateDepthBuffer(1)");
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	this->evaluateErrorsAndLog("generateDepthBuffer(2)");
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	this->evaluateErrorsAndLog("generateDepthBuffer(3)");
	return depthBuffer;
}

void OpenglES3Device::attachDepthBufferToFrameBufferAsRenderBuffer(unsigned int depthBuffer, unsigned int frameBuffer){
	this->bindFrameBuffer(frameBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	this->evaluateErrorsAndLogOnlyInDebugMode("attachDepthBufferToFrameBufferAsRenderBuffer");
}

void OpenglES3Device::attachDepthBufferToFrameBufferAsTexture(unsigned int depthBuffer, unsigned int frameBuffer){
	this->bindFrameBuffer(frameBuffer);
	if(depthBuffer != 0){
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
		this->evaluateErrorsAndLogOnlyInDebugMode("attachDepthBufferToFrameBufferAsTexture");
	}
}

void OpenglES3Device::bindTextureToFrameBuffer(ApiTexture* texture, unsigned int frameBufferHandler, unsigned int attachmentPosition){
	this->bindFrameBuffer(frameBufferHandler);
	string message = "AttachementNum:" + StringUtils::toString(texture->getTextureId());
	Logger::getInstance()->logError(new Log(message));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentPosition,
	GL_TEXTURE_2D, texture->getTextureId(), 0);
	this->evaluateErrorsAndLogOnlyInDebugMode("bindTextureToFrameBuffer");	
}

void OpenglES3Device::drawFrameBuffer(unsigned int frameBufferHandler, unsigned int colorAttachmentsCount){
	if(colorAttachmentsCount > 0){
		GLenum* drawBuffers = new GLenum[colorAttachmentsCount + 1]();
		for(unsigned int i = 0; i < colorAttachmentsCount; i++){
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glDrawBuffers(colorAttachmentsCount, drawBuffers);
		this->evaluateErrorsAndLogOnlyInDebugMode("drawFrameBuffer");
		delete drawBuffers;
	}
}

void OpenglES3Device::drawFrameBufferToBackBuffer(){
	GLenum drawBuffers[] = { GL_NONE, GL_BACK };
	glDrawBuffers(2, drawBuffers);
	this->evaluateErrorsAndLogOnlyInDebugMode("drawFrameBufferToBackBuffer");
}

void OpenglES3Device::bindFrameBuffer(unsigned int frameBufferHandler){
    if(frameBufferHandler == 0)
        frameBufferHandler = GraphicDevice::defaultFrameBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferHandler);
	this->evaluateErrorsAndLogOnlyInDebugMode("bindFrameBuffer");
	int result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(result != GL_FRAMEBUFFER_COMPLETE && result != GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT){
		//Estoy bindeando un framebuffer que no esta completo
		string message = "OpenGl ES 3 Warning: FBO incomplete in OpenglES3Device#bindFrameBuffer : glCheckFramebufferStatus result code: " + StringUtils::toString(result); 
		Logger::getInstance()->logError(new Log(message));
	}
}

unsigned int OpenglES3Device::getCurrentBindedFrameBuffer(){
	int bindedFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bindedFbo);
	this->evaluateErrorsAndLogOnlyInDebugMode("getCurrentBindedFrameBuffer");
	return (unsigned int) bindedFbo;
}


unsigned char* OpenglES3Device::getApiVersion(){
	return (unsigned char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
}

unsigned int OpenglES3Device::generateTexture(vec2 size, unsigned int internalFormat, unsigned int format, void* pixelData){
	unsigned int textureId;
	glGenTextures(1, &textureId);	
	this->evaluateErrorsAndLog("generateTexture(1)");
	glBindTexture(GL_TEXTURE_2D, textureId);	
	this->evaluateErrorsAndLog("generateTexture(2)");

	// REVIEW ANDROIDE: En el caso de crear un depth buffer 24 o 16 bits, tengo que usar un tipo int
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

void OpenglES3Device::setActiveTexture(unsigned int textureUnit){
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	this->evaluateErrorsAndLogOnlyInDebugMode("setActiveTexture");
}

void OpenglES3Device::bindTexture(unsigned int textureId){
	glBindTexture(GL_TEXTURE_2D, textureId);	
	this->evaluateErrorsAndLogOnlyInDebugMode("bindTexture");
}
void OpenglES3Device::setTextureParameterFor2DTexture(unsigned int parameterType, unsigned int value){
	glTexParameteri(GL_TEXTURE_2D, parameterType, value);
	this->evaluateErrorsAndLogOnlyInDebugMode("setTextureParameterFor2DTexture");
}
void OpenglES3Device::setTextureParameterFor2DTexture(unsigned int parameterType, float* value){
	//glTexParameterfv(GL_TEXTURE_2D, parameterType, value);
	this->evaluateErrorsAndLogOnlyInDebugMode("setTextureParameterFor2DTexture");
}
void OpenglES3Device::deleteTexture(unsigned int textureId){
	glDeleteTextures(1, &textureId);	
	this->evaluateErrorsAndLog("deleteTexture");
}
void OpenglES3Device::deleteFrameBuffer(unsigned int frameBuffer){
	glDeleteFramebuffers(1, &frameBuffer);
	this->evaluateErrorsAndLog("deleteFrameBuffer");
}
void OpenglES3Device::deleteDepthBuffer(unsigned int depthBuffer){
	glDeleteRenderbuffers(1, &depthBuffer);
	this->evaluateErrorsAndLog("deleteDepthBuffer");
}
void OpenglES3Device::generateMipMapFor2DTexture(int width, int height){
	glGenerateMipmap(GL_TEXTURE_2D);
	this->evaluateErrorsAndLog("generateMipMapFor2DTexture");
}

void OpenglES3Device::initiate(){
	// Opengl ES 3 es inicializado desde el conector de la ventana (ej con EGL).
}

void OpenglES3Device::destroy(){
	// Opengl ES 3 se destruye desde el conector de la ventana.
}

void OpenglES3Device::drawMesh(Mesh* mesh){
	this->drawMesh(mesh->getMeshBuffers());	
}
void OpenglES3Device::transformUsingFeedback(VertexArrayObject* vao, unsigned int vertexCount, TransformFeedbackObject* tfo){
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
void OpenglES3Device::drawMesh(MeshBuffers* meshBuffers){
	glBindVertexArray( meshBuffers->getVaoHandle());
	this->evaluateErrorsAndLogOnlyInDebugMode("drawMesh(1)");
	glDrawElements(GL_TRIANGLES, meshBuffers->getIndexBufferSize(), GL_UNSIGNED_INT, (GLvoid*)0);
	this->evaluateErrorsAndLogOnlyInDebugMode("drawMesh(2)");
}
void OpenglES3Device::drawUsingTransformFeedback(VertexArrayObject* vao, TransformFeedbackObject* tfo){
	// No parece estar soportado en opengl es 3, pero tampoco se usa en scaena actualmente.
	/*
	glBindVertexArray( vao->getHandler());
	this->evaluateErrorsAndLogOnlyInDebugMode("drawMeshWithTransformFeedback(1)");
	glDrawTransformFeedback(GL_TRIANGLES, tfo->getHandler());
	this->evaluateErrorsAndLogOnlyInDebugMode("drawMeshWithTransformFeedback(2)");
	*/
}
void OpenglES3Device::drawQuads(unsigned int vaoHandle, unsigned int vertexCount){
	// No puede renderizarse QUADS desde opengl ES. 
	// TODO: quizas deberia poner una excepcion aca.
}
void OpenglES3Device::drawTriangleStrip(unsigned int vaoHandle, unsigned int* indexBuffer, unsigned int vertexCount){
	glBindVertexArray( vaoHandle );
	this->evaluateErrorsAndLogOnlyInDebugMode("drawTriangleStrip(1)");
	glDrawElements(GL_TRIANGLE_STRIP, vertexCount, GL_UNSIGNED_INT, indexBuffer);
	this->evaluateErrorsAndLogOnlyInDebugMode("drawTriangleStrip(2)");
}
void OpenglES3Device::drawLine(unsigned int vaoHandle){
	glBindVertexArray(vaoHandle);
	this->evaluateErrorsAndLogOnlyInDebugMode("drawLine(1)");
	glDrawArrays( GL_LINES, 0, 2);		
	this->evaluateErrorsAndLogOnlyInDebugMode("drawLine(2)");
}
void OpenglES3Device::drawBuffer(unsigned int colorAttachmentNumber){
	unsigned int attachements[] = { GL_COLOR_ATTACHMENT0 + colorAttachmentNumber };
	glDrawBuffers(1, attachements );
	this->evaluateErrorsAndLogOnlyInDebugMode("drawBuffer");
}

unsigned int OpenglES3Device::generateVaoBuffer(){
	unsigned int handle;
	glGenVertexArrays( 1, &handle);
	this->evaluateErrorsAndLog("generateVaoBuffer(1)");
	return handle;
}
unsigned int OpenglES3Device::generateVboBuffer(){
	unsigned int handle;
	glGenBuffers(1, &handle);
	this->evaluateErrorsAndLog("generateVboBuffer(1)");
	return handle;
}
unsigned int OpenglES3Device::generateTransformFeedback(){
	unsigned int handle;
	glGenTransformFeedbacks(1, &handle);
	this->evaluateErrorsAndLog("generateTransformFeedback(1)");	
	return handle;
}

void OpenglES3Device::deleteTransformFeedback(unsigned int feedbackHandle){
	glDeleteTransformFeedbacks(1, &feedbackHandle);
}
void OpenglES3Device::deleteVboBuffer(unsigned int vboHandle){
	glDeleteBuffers(1, &vboHandle);
}
void OpenglES3Device::deleteVaoBuffer(unsigned int vaoHandle){
	glDeleteVertexArrays(1, &vaoHandle);
}

unsigned int OpenglES3Device::attachVboToTransformFeedback(unsigned int feedbackHandle, unsigned int vboHandle, unsigned int attachIndex){	
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedbackHandle);
	this->evaluateErrorsAndLog("attachVboToTransformFeedback(1)");
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, attachIndex, vboHandle);
	this->evaluateErrorsAndLog("attachVboToTransformFeedback(2)");
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	return feedbackHandle;
}

void OpenglES3Device::bindVaoBuffer(unsigned int vaoHandle){	
	glBindVertexArray( vaoHandle );
	this->evaluateErrorsAndLogOnlyInDebugMode("bindVaoBuffer(1)");
}


void OpenglES3Device::attachVboToVao(VertexBufferObject* vbo, unsigned int attachIndex){
	glBindBuffer( GL_ARRAY_BUFFER, vbo->getHandler() );
	glEnableVertexAttribArray(attachIndex);	
	this->evaluateErrorsAndLog("attachVboToVao(1)");
	glVertexAttribPointer( attachIndex, vbo->getComponentCount(), vbo->getDataType(), GL_FALSE, 0, (GLubyte*)NULL);
	this->evaluateErrorsAndLog("attachVboToVao(2)");	
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

// Carga un vertex buffer object en memoria de video
void OpenglES3Device::fillVboWithData(GraphicArrayContainer* container){
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

void OpenglES3Device::fillVboWithData(GraphicIndexArrayContainer* container){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, container->getVboHandle());
	this->evaluateErrorsAndLog("fillVboWithData(5)");
	unsigned int drawMethod = container->getDrawMethod(); 
	unsigned int dataSize = this->getArrayContainerDataTypeSize(container->getDataType());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, container->getBufferSize()*dataSize, container->getBuffer(), drawMethod);
	this->evaluateErrorsAndLog("fillVboWithData(6)");
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void OpenglES3Device::modifyGeometryArray(GraphicArrayContainer* container, unsigned int vboHandle){
	glBindBuffer( GL_ARRAY_BUFFER, vboHandle );
	this->evaluateErrorsAndLog("modifyGeometryArray(1)");
	unsigned int drawMethod = container->getDrawMethod();
	unsigned int dataSize = this->getArrayContainerDataTypeSize(container->getDataType());
	glBufferData( GL_ARRAY_BUFFER, container->getBufferSize() * dataSize, container->getBuffer(), drawMethod );
	this->evaluateErrorsAndLog("modifyGeometryArray(2)");
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

unsigned int OpenglES3Device::getArrayContainerDataTypeSize(unsigned int dataTypeInContainer){
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

void OpenglES3Device::evaluateErrorsAndLog(const char* location){
	unsigned int glErr = glGetError();
    if (glErr != GL_NO_ERROR)
	{
		//No tengo la descripcion del error, logueo el codigo
		string message = "OpenGl ES 3 error in OpenglES3Device#" + string(location) + " : " + "OpengGL ES 3 error code: " + StringUtils::toString( glErr ); 
		Logger::getInstance()->logError(new Log(message));
    }
}

void OpenglES3Device::evaluateErrorsAndLogOnlyInDebugMode(const char* location){
	if(VideoConfiguration::getInstance()->isDebugMode()){
		this->evaluateErrorsAndLog(location);
	}
}

// Para soporte de queries
unsigned int OpenglES3Device::generateQuery(){
	unsigned int query;
	glGenQueries(1, &query);
	this->evaluateErrorsAndLog("generateQuery(1)");
	return query;
}
void OpenglES3Device::beginQuery(unsigned int queryHandler, unsigned int queryType){
	/*
	glBeginQuery(queryType, queryHandler);
	this->evaluateErrorsAndLogOnlyInDebugMode("beginQuery(1)");
	*/
}
void OpenglES3Device::endQuery(unsigned int queryType){
	/*
	glEndQuery(queryType);
	this->evaluateErrorsAndLogOnlyInDebugMode("endQuery(1)");
	*/
}
unsigned int OpenglES3Device::getQueryResultAsUInt(unsigned int queryHandler){
	/*
	// Aca no hago chequeo de errores porque en los casos como TIME_QUERY, no esta soportado en ES pero llega hasta aca y me llena el log de basura
	GLuint done = 0;
	while (!done) {
		glGetQueryObjectuiv(queryHandler, GL_QUERY_RESULT_AVAILABLE, &done);
	}
	unsigned int queryResult;
	glGetQueryObjectuiv(queryHandler, GL_QUERY_RESULT, &queryResult);
	return queryResult;
	*/
	return 0;
}

void OpenglES3Device::deleteQuery(unsigned int queryHandler){
	glDeleteQueries(1, &queryHandler);
	this->evaluateErrorsAndLog("deleteQuery(1)");
}
