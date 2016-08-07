/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#define GL_GLEXT_PROTOTYPES 1 // TODO: Falta verificar que se cuente con estas extensiones
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <glm/glm.hpp>

#include "OpenglES2Device.h"
#include "GraphicInitializationException.h"
#include "ApiVariableTypeDoesNotExistException.h"
#include "OpenglES2ConstantCatalog.h"
#include "Logger.h"
#include "VideoConfiguration.h"
#include "StringUtils.h"
#include "FeatureNotAvaibleException.h"

OpenglES2Device::OpenglES2Device(){
    this->constantCatalog = new OpenglES2ConstantCatalog();
}

void OpenglES2Device::setViewport(unsigned int x, unsigned int y, int width, int height){
    glViewport(x, y, width, height);
    this->evaluateErrorsAndLogOnlyInDebugMode("setViewport");
}
void OpenglES2Device::enableMode(unsigned int mode){
    glEnable(mode);
    this->evaluateErrorsAndLogOnlyInDebugMode("enableMode");
}
void OpenglES2Device::disableMode(unsigned int mode){
    glDisable(mode);
    this->evaluateErrorsAndLogOnlyInDebugMode("disableMode");
}

void OpenglES2Device::frontFaceSelector(unsigned int faceFlag){
    glFrontFace(faceFlag);
    this->evaluateErrorsAndLogOnlyInDebugMode("frontFaceSelector");
}

// Limpieza del buffer activo
void OpenglES2Device::clearColorBuffer(){
    glClear(GL_COLOR_BUFFER_BIT);
    this->evaluateErrorsAndLogOnlyInDebugMode("clearColorBuffer");
}
void OpenglES2Device::clearDepthBuffer(){
    glClear(GL_DEPTH_BUFFER_BIT);
    this->evaluateErrorsAndLogOnlyInDebugMode("clearDepthBuffer");
}
void OpenglES2Device::clearColorAndDepthBuffer(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->evaluateErrorsAndLogOnlyInDebugMode("clearColorAndDepthBuffer");
}

// Setea el color con el que limpiar
void OpenglES2Device::clearColor(float r, float g, float b, float a){
    glClearColor(r, g, b, a);
    this->evaluateErrorsAndLogOnlyInDebugMode("clearColor");
}

// Para definir como se procesa el blend
void OpenglES2Device::blendFunction(unsigned int sFactor, unsigned int dFactor){
    glBlendFunc (sFactor, dFactor);
    this->evaluateErrorsAndLogOnlyInDebugMode("defineBlendFunction");
}

unsigned int OpenglES2Device::generateFrameBuffer(){
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    this->evaluateErrorsAndLog("generateFrameBuffer");
    return fbo;
}

unsigned int OpenglES2Device::generateDepthBuffer(int width, int height){
    unsigned int depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    this->evaluateErrorsAndLog("generateDepthBuffer(1)");
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    this->evaluateErrorsAndLog("generateDepthBuffer(2)");
    glRenderbufferStorage(GL_RENDERBUFFER, this->getConstantCatalog()->getFormatDepth24(), width, height);
    this->evaluateErrorsAndLog("generateDepthBuffer(3)");
    return depthBuffer;
}

void OpenglES2Device::attachDepthBufferToFrameBufferAsRenderBuffer(unsigned int depthBuffer, unsigned int frameBuffer){
    this->bindFrameBuffer(frameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    this->evaluateErrorsAndLogOnlyInDebugMode("attachDepthBufferToFrameBufferAsRenderBuffer");
}

void OpenglES2Device::attachDepthBufferToFrameBufferAsTexture(unsigned int depthBuffer, unsigned int frameBuffer){
    this->bindFrameBuffer(frameBuffer);
    if(depthBuffer != 0){
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
        this->evaluateErrorsAndLogOnlyInDebugMode("attachDepthBufferToFrameBufferAsTexture");
    }
}

void OpenglES2Device::bindTextureToFrameBuffer(ApiTexture* texture, unsigned int frameBufferHandler, unsigned int attachmentPosition){
    if(attachmentPosition != 0){
        throw new FeatureNotAvaibleException("OpenGL ES 2 no soporta mas de un color attachment");
    }
    this->bindFrameBuffer(frameBufferHandler);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getTextureId(), 0);
    this->evaluateErrorsAndLogOnlyInDebugMode("bindTextureToFrameBuffer");
    this->bindFrameBuffer(frameBufferHandler);
}

void OpenglES2Device::drawFrameBuffer(unsigned int frameBufferHandler, unsigned int colorAttachmentsCount){
    // Opengl es 2 no soporta mas de un attachement color
    this->drawBuffer(colorAttachmentsCount);
}

void OpenglES2Device::drawFrameBufferToBackBuffer(){
    // OpenGL ES 2 no soporta mas de un attachement color, por defecto defaultea
    //GLenum drawBuffers[] = { GL_NONE, GL_BACK };
    //glDrawBuffers(2, drawBuffers);
    //this->evaluateErrorsAndLogOnlyInDebugMode("drawFrameBufferToBackBuffer");
}

void OpenglES2Device::bindFrameBuffer(unsigned int frameBufferHandler){
    if(frameBufferHandler == 0)
        frameBufferHandler = GraphicDevice::defaultFrameBuffer;
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferHandler);
    this->evaluateErrorsAndLogOnlyInDebugMode("bindFrameBuffer");
    int result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(result != GL_FRAMEBUFFER_COMPLETE && result != GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT){
        //Estoy bindeando un framebuffer que no esta completo
        string message = "OpenGl ES 2 Warning: FBO incomplete in OpenglES2Device#bindFrameBuffer : glCheckFramebufferStatus result code: " + StringUtils::toString(result);
        Logger::getInstance()->logError(new Log(message));
    }
}

unsigned int OpenglES2Device::getCurrentBindedFrameBuffer(){
    int bindedFbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bindedFbo);
    this->evaluateErrorsAndLogOnlyInDebugMode("getCurrentBindedFrameBuffer");
    return (unsigned int) bindedFbo;
}


unsigned char* OpenglES2Device::getApiVersion(){
    return (unsigned char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
}

unsigned int OpenglES2Device::generateTexture(vec2 size, unsigned int internalFormat, unsigned int format, void* pixelData){
    unsigned int textureId;
    glGenTextures(1, &textureId);
    this->evaluateErrorsAndLog("generateTexture(1)");
    glBindTexture(GL_TEXTURE_2D, textureId);
    this->evaluateErrorsAndLog("generateTexture(2)");
    
    GraphicDeviceConstantCatalog* catalog = this->getConstantCatalog();
    
    // REVIEW ANDROIDE: En el caso de crear un depth buffer 24 o 16 bits, tengo que usar un tipo int
    unsigned int type = GL_UNSIGNED_BYTE;
    if(internalFormat == catalog->getFormatDepth24() || internalFormat == GL_DEPTH_COMPONENT16){
        type = GL_UNSIGNED_INT;
    }
    else if(internalFormat == catalog->getFormatRGB32F() || internalFormat == GL_RGB16F_EXT || internalFormat == catalog->getFormatRG32F()
            || internalFormat == GL_RG16F_EXT  || internalFormat == catalog->getFormatR32F() || internalFormat == GL_R16F_EXT
            || internalFormat == GL_RGBA32F_EXT || internalFormat == GL_RGBA16F_EXT){
        type = GL_FLOAT;
    }
    // En OpenGL ES 2 usamos para el formato interno el mismo que el externo (documentacion dice que tiene eso)
    // Ademas en los frame buffer attachments no podemos usar GL_FLOAT como tipo
    if(pixelData == NULL){
        type = GL_UNSIGNED_BYTE;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, type, pixelData);
    this->evaluateErrorsAndLog("generateTexture(3)");
    return textureId;
}

void OpenglES2Device::setActiveTexture(unsigned int textureUnit){
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    this->evaluateErrorsAndLogOnlyInDebugMode("setActiveTexture");
}

void OpenglES2Device::bindTexture(unsigned int textureId){
    glBindTexture(GL_TEXTURE_2D, textureId);
    this->evaluateErrorsAndLogOnlyInDebugMode("bindTexture");
}
void OpenglES2Device::setTextureParameterFor2DTexture(unsigned int parameterType, unsigned int value){
    glTexParameteri(GL_TEXTURE_2D, parameterType, value);
    this->evaluateErrorsAndLogOnlyInDebugMode("setTextureParameterFor2DTexture");
}
void OpenglES2Device::setTextureParameterFor2DTexture(unsigned int parameterType, float* value){
    //glTexParameterfv(GL_TEXTURE_2D, parameterType, value);
    this->evaluateErrorsAndLogOnlyInDebugMode("setTextureParameterFor2DTexture");
}
void OpenglES2Device::deleteTexture(unsigned int textureId){
    glDeleteTextures(1, &textureId);
    this->evaluateErrorsAndLog("deleteTexture");
}
void OpenglES2Device::deleteFrameBuffer(unsigned int frameBuffer){
    glDeleteFramebuffers(1, &frameBuffer);
    this->evaluateErrorsAndLog("deleteFrameBuffer");
}
void OpenglES2Device::deleteDepthBuffer(unsigned int depthBuffer){
    glDeleteRenderbuffers(1, &depthBuffer);
    this->evaluateErrorsAndLog("deleteDepthBuffer");
}
bool isPowerOfTwo(int x) {
    return (x != 0) && ((x & (x - 1)) == 0);
}
void OpenglES2Device::generateMipMapFor2DTexture(int width, int height){
    // Opengl ES 2 requiere que para aplicar mipmap las texturas sean potencias de 2
    if(VideoConfiguration::getInstance()->isMipMapActive() && isPowerOfTwo(width) && isPowerOfTwo(height) ){
        glGenerateMipmap(GL_TEXTURE_2D);
        this->evaluateErrorsAndLog("generateMipMapFor2DTexture");
    }
}

void OpenglES2Device::initiate(){
    // Opengl ES 3 es inicializado desde el conector de la ventana (ej con EGL).
}

void OpenglES2Device::destroy(){
    // Opengl ES 3 se destruye desde el conector de la ventana.
}

void OpenglES2Device::drawMesh(Mesh* mesh){
    this->drawMesh(mesh->getMeshBuffers());
}
void OpenglES2Device::transformUsingFeedback(VertexArrayObject* vao, unsigned int vertexCount, TransformFeedbackObject* tfo){
    throw new FeatureNotAvaibleException("OpenGL ES 2.0: No esta disponible el transform feedback");
}
void OpenglES2Device::drawMesh(MeshBuffers* meshBuffers){
    glBindVertexArrayOES( meshBuffers->getVaoHandle());
    this->evaluateErrorsAndLogOnlyInDebugMode("drawMesh(1)");
    glDrawElements(GL_TRIANGLES, meshBuffers->getIndexBufferSize(), GL_UNSIGNED_INT, (GLvoid*)0);
    this->evaluateErrorsAndLogOnlyInDebugMode("drawMesh(2)");
}
void OpenglES2Device::drawUsingTransformFeedback(VertexArrayObject* vao, TransformFeedbackObject* tfo){
    throw new FeatureNotAvaibleException("OpenGL ES 2.0: No esta disponible el transform feedback");
}
void OpenglES2Device::drawQuads(unsigned int vaoHandle, unsigned int vertexCount){
    // No puede renderizarse QUADS desde opengl ES.
    // TODO: quizas deberia poner una excepcion aca.
}
void OpenglES2Device::drawTriangleStrip(unsigned int vaoHandle, unsigned int* indexBuffer, unsigned int vertexCount){
    glBindVertexArrayOES( vaoHandle );
    this->evaluateErrorsAndLogOnlyInDebugMode("drawTriangleStrip(1)");
    glDrawElements(GL_TRIANGLE_STRIP, vertexCount, GL_UNSIGNED_INT, indexBuffer);
    this->evaluateErrorsAndLogOnlyInDebugMode("drawTriangleStrip(2)");
}
void OpenglES2Device::drawLine(unsigned int vaoHandle){
    glBindVertexArrayOES(vaoHandle);
    this->evaluateErrorsAndLogOnlyInDebugMode("drawLine(1)");
    glDrawArrays( GL_LINES, 0, 2);
    this->evaluateErrorsAndLogOnlyInDebugMode("drawLine(2)");
}
void OpenglES2Device::drawBuffer(unsigned int colorAttachmentNumber){
    // OpenGL ES 2 no soporta multiples color attachements, simplemente defaultea a cero
    if(colorAttachmentNumber > 1)
        throw new FeatureNotAvaibleException("OpenGL ES 2 no soporta mas de 1 attachement color");
}

unsigned int OpenglES2Device::generateVaoBuffer(){
    unsigned int handle;
    glGenVertexArraysOES( 1, &handle);
    this->evaluateErrorsAndLog("generateVaoBuffer(1)");
    return handle;
}
unsigned int OpenglES2Device::generateVboBuffer(){
    unsigned int handle;
    glGenBuffers(1, &handle);
    this->evaluateErrorsAndLog("generateVboBuffer(1)");
    return handle;
}
unsigned int OpenglES2Device::generateTransformFeedback(){
    throw new FeatureNotAvaibleException("OpenGL ES 2.0: No esta disponible el transform feedback");
}

void OpenglES2Device::deleteTransformFeedback(unsigned int feedbackHandle){
    throw new FeatureNotAvaibleException("OpenGL ES 2.0: No esta disponible el transform feedback");
}
void OpenglES2Device::deleteVboBuffer(unsigned int vboHandle){
    glDeleteBuffers(1, &vboHandle);
}
void OpenglES2Device::deleteVaoBuffer(unsigned int vaoHandle){
    glDeleteVertexArraysOES(1, &vaoHandle);
}

unsigned int OpenglES2Device::attachVboToTransformFeedback(unsigned int feedbackHandle, unsigned int vboHandle, unsigned int attachIndex){
    throw new FeatureNotAvaibleException("OpenGL ES 2.0: No esta disponible el transform feedback");
}

void OpenglES2Device::bindVaoBuffer(unsigned int vaoHandle){
    glBindVertexArrayOES( vaoHandle );
    this->evaluateErrorsAndLogOnlyInDebugMode("bindVaoBuffer(1)");
}


void OpenglES2Device::attachVboToVao(VertexBufferObject* vbo, unsigned int attachIndex){
    glBindBuffer( GL_ARRAY_BUFFER, vbo->getHandler() );
    glEnableVertexAttribArray(attachIndex);
    this->evaluateErrorsAndLog("attachVboToVao(1)");
    glVertexAttribPointer( attachIndex, vbo->getComponentCount(), vbo->getDataType(), GL_FALSE, 0, (GLubyte*)NULL);
    this->evaluateErrorsAndLog("attachVboToVao(2)");
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

// Carga un vertex buffer object en memoria de video
void OpenglES2Device::fillVboWithData(GraphicArrayContainer* container){
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

void OpenglES2Device::fillVboWithData(GraphicIndexArrayContainer* container){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, container->getVboHandle());
    this->evaluateErrorsAndLog("fillVboWithData(5)");
    unsigned int drawMethod = container->getDrawMethod();
    unsigned int dataSize = this->getArrayContainerDataTypeSize(container->getDataType());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, container->getBufferSize()*dataSize, container->getBuffer(), drawMethod);
    this->evaluateErrorsAndLog("fillVboWithData(6)");
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void OpenglES2Device::modifyGeometryArray(GraphicArrayContainer* container, unsigned int vboHandle){
    glBindBuffer( GL_ARRAY_BUFFER, vboHandle );
    this->evaluateErrorsAndLog("modifyGeometryArray(1)");
    unsigned int drawMethod = container->getDrawMethod();
    unsigned int dataSize = this->getArrayContainerDataTypeSize(container->getDataType());
    glBufferData( GL_ARRAY_BUFFER, container->getBufferSize() * dataSize, container->getBuffer(), drawMethod );
    this->evaluateErrorsAndLog("modifyGeometryArray(2)");
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

unsigned int OpenglES2Device::getArrayContainerDataTypeSize(unsigned int dataTypeInContainer){
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

void OpenglES2Device::evaluateErrorsAndLog(const char* location){
    unsigned int glErr = glGetError();
    if (glErr != GL_NO_ERROR)
    {
        //No tengo la descripcion del error, logueo el codigo
        string message = "OpenGl ES 2 error in OpenglES2Device#" + string(location) + " : " + "OpengGL ES 3 error code: " + StringUtils::toString( glErr );
        Logger::getInstance()->logError(new Log(message));
    }
}

void OpenglES2Device::evaluateErrorsAndLogOnlyInDebugMode(const char* location){
    if(VideoConfiguration::getInstance()->isDebugMode()){
        this->evaluateErrorsAndLog(location);
    }
}

// Para soporte de queries
unsigned int OpenglES2Device::generateQuery(){
    /*  REVIEW: Me parece que openglES 2 no soporta queries */
    return 0;
}
void OpenglES2Device::beginQuery(unsigned int queryHandler, unsigned int queryType){
    /*
     glBeginQuery(queryType, queryHandler);
     this->evaluateErrorsAndLogOnlyInDebugMode("beginQuery(1)");
     */
}
void OpenglES2Device::endQuery(unsigned int queryType){
    /*
     glEndQuery(queryType);
     this->evaluateErrorsAndLogOnlyInDebugMode("endQuery(1)");
     */
}
unsigned int OpenglES2Device::getQueryResultAsUInt(unsigned int queryHandler){
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

void OpenglES2Device::deleteQuery(unsigned int queryHandler){
    //glDeleteQueries(1, &queryHandler);
    //this->evaluateErrorsAndLog("deleteQuery(1)");
}

