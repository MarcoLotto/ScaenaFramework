/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GraphicDevice.h"

class OpenglES2Device : public GraphicDevice{
private:
	unsigned int getArrayContainerDataTypeSize(unsigned int dataTypeInContainer);
	void evaluateErrorsAndLog(const char* location);
	void evaluateErrorsAndLogOnlyInDebugMode(const char* location);

public:
	OpenglES2Device();
	void initiate();
	void destroy();

	// Limpieza del buffer activo
	void clearColorBuffer();
	void clearDepthBuffer();
	void clearColorAndDepthBuffer();

	// Setea el color con el que limpiar
	void clearColor(float r, float g, float b, float a);

	// Activar caracteristicas especiales del API como transparecia o culling
	void enableMode(unsigned int mode);
	void disableMode(unsigned int mode);

	// Para definir como se procesa el blend
	void blendFunction(unsigned int sFactor, unsigned int dFactor);

	// Selecciona que cara de los poligonos utilizar como cara frontal
	void frontFaceSelector(unsigned int faceFlag);

	// Setea el tamaño y posicion del viewport activo
	void setViewport(unsigned int x, unsigned int y, int width, int height);
	
	unsigned int generateFrameBuffer();
	unsigned int generateDepthBuffer(int width, int height);
	void attachDepthBufferToFrameBufferAsRenderBuffer(unsigned int depthBuffer, unsigned int frameBuffer);
	void attachDepthBufferToFrameBufferAsTexture(unsigned int depthBuffer, unsigned int frameBuffer);
	void bindTextureToFrameBuffer(ApiTexture* texture, unsigned int frameBufferHandler, unsigned int attachmentPosition);
	void drawFrameBuffer(unsigned int frameBufferHandler, unsigned int colorAttachmentsCount);
	void drawFrameBufferToBackBuffer();
	void bindFrameBuffer(unsigned int frameBufferHandler);
	void deleteFrameBuffer(unsigned int frameBuffer);
	void deleteDepthBuffer(unsigned int depthBuffer);
	unsigned int getCurrentBindedFrameBuffer();

	unsigned char* getApiVersion();

	unsigned int generateTexture(vec2 size, unsigned int internalFormat, unsigned int format, void* pixelData);
	void setActiveTexture(unsigned int textureUnit);
	void bindTexture(unsigned int textureId);
	void setTextureParameterFor2DTexture(unsigned int parameterType, unsigned int value);
	void setTextureParameterFor2DTexture(unsigned int parameterType, float* value);
	void deleteTexture(unsigned int textureId);
	void generateMipMapFor2DTexture(int width, int height);
	
	void drawMesh(Mesh* mesh);
	void drawMesh(MeshBuffers* meshBuffers);
	void transformUsingFeedback(VertexArrayObject* vao, unsigned int vertexCount, TransformFeedbackObject* tfo);
	void drawUsingTransformFeedback(VertexArrayObject* vao, TransformFeedbackObject* tfo);
	void drawQuads(unsigned int vaoHandle, unsigned int vertexCount);
	void drawTriangleStrip(unsigned int vaoHandle, unsigned int* indexBuffer, unsigned int vertexCount);
	void drawLine(unsigned int vaoHandle);
	void drawBuffer(unsigned int colorAttachmentNumber);

	// Para crear los buffers en memoria
	unsigned int generateVaoBuffer();
	unsigned int generateVboBuffer();
	unsigned int generateTransformFeedback();

	// Para borrar los buffers de memoria
	void deleteTransformFeedback(unsigned int feedbackHandle);
	void deleteVboBuffer(unsigned int vboHandle);
	void deleteVaoBuffer(unsigned int vaoHandle);

	// Attachea el vbo dado al feedback handle, en la posicion attachIndex
	unsigned int attachVboToTransformFeedback(unsigned int feedbackHandle, unsigned int vboHandle, unsigned int attachIndex);
	// Attach de un VBO a un VAO. Requiere el VBO y la posicion en el VAO a la que se atachea. Se debe bindear previamente el VAO.
	void attachVboToVao(VertexBufferObject* vbo, unsigned int attachIndex);
	// Bindea un vao buffer al contexto
	void bindVaoBuffer(unsigned int vaoHandle);
	// Carga un vertex buffer object en memoria de video
	void fillVboWithData(GraphicArrayContainer* container);
	void fillVboWithData(GraphicIndexArrayContainer* container);
	
	// Modifica la data de un VBO
	virtual void modifyGeometryArray(GraphicArrayContainer* container, unsigned int vboHandle);
			
	// Para soporte de queries
	unsigned int generateQuery();
	void beginQuery(unsigned int queryHandler, unsigned int queryType);
	void endQuery(unsigned int queryType);
	unsigned int getQueryResultAsUInt(unsigned int queryHandler);
	void deleteQuery(unsigned int queryHandler);
};