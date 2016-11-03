/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "FrameBuffer.h"
#include "DepthTexture.h"
#include "FrameBufferRequest.h"
#include <list>
#include <map>
using namespace std;

class FrameBufferManager{

private:
	list<FrameBuffer*> frameBuffers;
	list<ApiTexture*> avaibleTextures;
	list<DepthTexture*> avaibleDepthBuffers;
	map<MemoryTexture*, unsigned int> textureReferences;
	static FrameBufferManager* instance;
	unsigned int garbageDaemonSleepTimeSlot;
	unsigned int aplicationConsideredInPauseTimeSlot;
	FrameBuffer* toScreenFrameBuffer;
	FrameBuffer* activeFrameBuffer;

	FrameBuffer* buildFrameBufferFromRequest(FrameBufferRequest* request);
	ApiTexture* getApiTextureForRequest(TextureRequest* request);
	ApiTexture* createApiTexture(TextureRequest* request);
	DepthTexture* getDepthBuffer(TextureRequest* request);
	FrameBufferManager();
	
	void deleteFrameBufferResources(FrameBuffer* frameBuffer);
	void logTextureReference(MemoryTexture* texture);
	void memoryCleanerDaemon();

public:
	static FrameBufferManager* getInstance();
	virtual ~FrameBufferManager();

	// Pido un frame buffer. Request indica el tipo de frame buffer y texturas a
	// obtener. UsedFrameBuffer indica el ultimo frame buffer devuelto, si es null
	// esta informacion se obtiene del request. Tener en cuenta que si usedFrameBuffer
	// no es null (y es valido), se ignora el request y se devuelve el mismo frameBuffer.
	// Al finalizar se bindea el frame buffer. Opcionalmente se informa si se debe devolver
	// un framebuffer que renderice en la pantalla.
	FrameBuffer* getFrameBufferAndBind(FrameBufferRequest* request, FrameBuffer* usedFrameBuffer, bool toScreenRender = false);

	// Devuelve un nuevo frame buffer el cual no será manejado por el FrameBufferManager. Esto trae como consecuencia que 
	// el manager no manejará el borrado ni reutilización del mismo, por lo que se debe borrar los recursos de manera externa.
	// TODO: Falta que tome en cuenta si hay recursos libres y disponibles para usar en vez de crear todo de cero. 
	FrameBuffer* getUnmanagedFrameBufferAndBind(FrameBufferRequest* request);

	// Devuelve el frame buffer que esta bindeado actualmente
	FrameBuffer* getActiveFrameBuffer();

	// Se limpian todos los frame buffers independientemente de si tienen writelock o no.
	// Se borran tambien sus texturas y recursos asociados.
	void clearAllFrameBuffers();

	// Limpia todos los frame buffers que no fueron usados en un rato (y sus recursos)
	void cleanAllUnusedResorces();

	// Limpia aquellos frame buffers y sus recursos que no tienen writelock actualmente
	// Esto puede ser util si yo entro en un menu y quiero borrar rapidamente lo anterior
	// sin borrar los framebuffers que estoy usando justo ahora.
	void cleanAllNonWriteLockedFrameBuffers();

	// Se informa que un frame buffer fue bindeado externamente al manager. Es usado por los frame buffers.
	void informBindedFrameBuffer(FrameBuffer* bindedFrameBuffer);
};