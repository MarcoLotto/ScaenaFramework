/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ApiTexture.h"
#include "Mesh.h"
#include "GraphicArrayContainer.h"
#include "GraphicIndexArrayContainer.h"
#include "GraphicDeviceConstantCatalog.h"
#include "GLSLProgram.h"

class GraphicDevice{
private:
	static GraphicDevice* instance;
    static int bindedApi;
    
protected:
    static unsigned int defaultFrameBuffer;
    GraphicDeviceConstantCatalog* constantCatalog;

	virtual void evaluateErrorsAndLog(const char* location) = 0;
	virtual void evaluateErrorsAndLogOnlyInDebugMode(const char* location) = 0;

public:
	//Selecciona el api a utilizar
	static void bindApi(int apiId);
    
    // Devuelve el id de api que se bindeo por ultima vez
    static int getBindedApi(){ return bindedApi; }
    
    // Setea el framebuffer por default para dibujar en la pantalla si es que el mismo es diferente de cero
    static void setDefaultFrameBuffer(unsigned int frameBufferHandler);
  	
	virtual void initiate()=0;
	virtual void destroy()=0;

	// Limpieza del buffer activo
	virtual void clearColorBuffer() = 0;
	virtual void clearDepthBuffer() = 0;
	virtual void clearColorAndDepthBuffer() = 0;

	// Setea el color con el que limpiar
	virtual void clearColor(float r, float g, float b, float a) = 0;

	// Activar caracteristicas especiales del API como transparecia o culling
	virtual void enableMode(unsigned int mode)=0;
	virtual void disableMode(unsigned int mode)=0;

	// Para definir como se procesa el blend
	virtual void blendFunction(unsigned int sFactor, unsigned int dFactor) = 0;

	//Pido el graphicDevice activo (tiene que haberse bindeado un api antes)
	static GraphicDevice* getInstance();
	GraphicDeviceConstantCatalog* getConstantCatalog(){ return this->constantCatalog; }
	virtual unsigned char* getApiVersion() = 0;

	// Selecciona que cara de los poligonos utilizar como cara frontal
	virtual void frontFaceSelector(unsigned int faceFlag)=0;

	// Setea el tamaño y posicion del viewport activo
	virtual void setViewport(unsigned int x, unsigned int y, int width, int height)=0;

	virtual unsigned int generateFrameBuffer()=0;
	virtual unsigned int generateDepthBuffer(int width, int height) = 0;
	virtual void attachDepthBufferToFrameBufferAsRenderBuffer(unsigned int depthBuffer, unsigned int frameBuffer) = 0;
	virtual void attachDepthBufferToFrameBufferAsTexture(unsigned int depthBuffer, unsigned int frameBuffer) = 0;
	virtual void bindTextureToFrameBuffer(ApiTexture* texture, unsigned int frameBufferHandler, unsigned int attachmentPosition)=0;
	virtual void drawFrameBuffer(unsigned int frameBufferHandler, unsigned int colorAttachmentsCount)=0;
	virtual void drawFrameBufferToBackBuffer()=0;
	virtual void bindFrameBuffer(unsigned int frameBufferHandler)=0;
	virtual void deleteFrameBuffer(unsigned int frameBuffer)=0;
	virtual void deleteDepthBuffer(unsigned int depthBuffer)=0;
	virtual unsigned int getCurrentBindedFrameBuffer()=0;

	virtual unsigned int generateTexture(vec2 size, unsigned int internalFormat, unsigned int format, void* pixelData)=0;
	virtual void setActiveTexture(unsigned int textureUnit)=0;
	virtual void bindTexture(unsigned int textureId)=0;
	virtual void setTextureParameterFor2DTexture(unsigned int parameterType, unsigned int value)=0;
	virtual void setTextureParameterFor2DTexture(unsigned int parameterType, float* value)=0;
	virtual void deleteTexture(unsigned int textureId)=0;
	virtual void generateMipMapFor2DTexture(int width, int height) = 0;

	virtual void drawMesh(Mesh* mesh) = 0;
	virtual void drawMesh(MeshBuffers* meshBuffers) = 0;
	virtual void transformUsingFeedback(VertexArrayObject* vao, unsigned int vertexCount, TransformFeedbackObject* tfo) = 0;
	virtual void drawUsingTransformFeedback(VertexArrayObject* vao, TransformFeedbackObject* tfo) = 0;
	virtual void drawQuads(unsigned int vaoHandle, unsigned int vertexCount) = 0;
	virtual void drawTriangleStrip(unsigned int vaoHandle, unsigned int* indexBuffer, unsigned int vertexCount) = 0;
	virtual void drawLine(unsigned int vaoHandle) = 0;
	virtual void drawBuffer(unsigned int colorAttachmentNumber) = 0;

	// Para crear los buffers en memoria
	virtual unsigned int generateVaoBuffer() = 0;
	virtual unsigned int generateVboBuffer() = 0;
	virtual unsigned int generateTransformFeedback() = 0;

	// Para borrar los buffers de memoria
	virtual void deleteTransformFeedback(unsigned int feedbackHandle) = 0;
	virtual void deleteVboBuffer(unsigned int vboHandle) = 0;
	virtual void deleteVaoBuffer(unsigned int vaoHandle) = 0;

	// Attachea el vbo dado al feedback handle, en la posicion attachIndex
	virtual unsigned int attachVboToTransformFeedback(unsigned int feedbackHandle, unsigned int vboHandle, unsigned int attachIndex) = 0;
	// Attach de un VBO a un VAO. Requiere el VBO y la posicion en el VAO a la que se atachea. Se debe bindear previamente el VAO.
	virtual void attachVboToVao(VertexBufferObject* vbo, unsigned int attachIndex) = 0;
	// Bindea un vao buffer al contexto
	virtual void bindVaoBuffer(unsigned int vaoHandle) = 0;
	// Carga un vertex buffer object en memoria de video
	virtual void fillVboWithData(GraphicArrayContainer* container) = 0;
	virtual void fillVboWithData(GraphicIndexArrayContainer* container) = 0;

	// Modifica la data de un VBO
	virtual void modifyGeometryArray(GraphicArrayContainer* container, unsigned int vboHandle) = 0;

	// Para soporte de queries
	virtual unsigned int generateQuery() = 0;
	virtual void beginQuery(unsigned int queryHandler, unsigned int queryType) = 0;
	virtual void endQuery(unsigned int queryType) = 0;
	virtual unsigned int getQueryResultAsUInt(unsigned int queryHandler) = 0;
	virtual void deleteQuery(unsigned int queryHandler) = 0;
};