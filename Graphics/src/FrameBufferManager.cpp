/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "FrameBufferManager.h"
#include "ToScreenFrameBuffer.h"

FrameBufferManager* FrameBufferManager::instance = NULL;

// Define el tiempo despues del cual, un frame buffer que no fue bindeado, es borrado
#define MAX_UNNUSED_TIME_IN_MILLISECONDS 5000 
// Define el tiempo que tarda en despertarse el daemon recolector de basura
#define DAEMON_SLEEP_TIME 5000
// Define el tiempo para el cual se considera que la aplicacion esta en pausa (para evitar borrar cosas)
#define APLICATION_PAUSE_TIME 1000

FrameBufferManager* FrameBufferManager::getInstance(){
	if(instance == NULL)
		instance = new FrameBufferManager();
	return instance;
}

FrameBufferManager::FrameBufferManager(){
	this->garbageDaemonSleepTimeSlot = TimeManager::getInstance()->reservTimeSlot();
	this->aplicationConsideredInPauseTimeSlot = TimeManager::getInstance()->reservTimeSlot();
	this->toScreenFrameBuffer = new ToScreenFrameBuffer();
	this->activeFrameBuffer = this->toScreenFrameBuffer;
}

FrameBufferManager::~FrameBufferManager(){
	TimeManager::getInstance()->removeTimeSlot(this->garbageDaemonSleepTimeSlot);
	TimeManager::getInstance()->removeTimeSlot(this->aplicationConsideredInPauseTimeSlot);
	instance = NULL;
	delete this->toScreenFrameBuffer;
}

FrameBuffer* FrameBufferManager::getFrameBufferAndBind(FrameBufferRequest* request, FrameBuffer* usedFrameBuffer, bool toScreenRender){
	// Invoco al daemon recolector de basura de frame buffers (tenia que colgarlo de algun lugar)
	this->memoryCleanerDaemon();

	// Si se indica que se renderiza a la pantalla, devuelvo el framebuffer que va a la pantalla
	if(toScreenRender){
		this->toScreenFrameBuffer->bindAndLockOutputs();		
		return this->toScreenFrameBuffer;
	}
	//Si used frame buffer es valido, no busco nada y devuelvo el mismo (lo que usualmente deberia pasar)
	if((usedFrameBuffer != NULL) && (!usedFrameBuffer->hasWriteLock()) && usedFrameBuffer->areHashCodesEqual(request->getHashCode())){
		usedFrameBuffer->bindAndLockOutputs();		
		return usedFrameBuffer;
	}
	//Busco si hay algun frame buffer que se adapte al request pedido
	std::list<FrameBuffer*>::iterator it = this->frameBuffers.begin();
	while(it != this->frameBuffers.end()){
		FrameBuffer* frameBuffer = (*it);
		if((request->matches(frameBuffer)) && (!frameBuffer->hasWriteLock())){
			frameBuffer->bindAndLockOutputs();			
			return frameBuffer;
		}
		++it;
	}
	//No tengo nada que se adapte, creo un frame buffer nuevo (y lo bindeo)
	FrameBuffer* frameBuffer = this->buildFrameBufferFromRequest(request);
	this->frameBuffers.push_back(frameBuffer);	
	frameBuffer->bindAndLockOutputs();
	return frameBuffer;
}

FrameBuffer* FrameBufferManager::getActiveFrameBuffer(){
	return this->activeFrameBuffer;
}

void FrameBufferManager::informBindedFrameBuffer(FrameBuffer* bindedFrameBuffer){
	this->activeFrameBuffer = bindedFrameBuffer;
}

FrameBuffer* FrameBufferManager::buildFrameBufferFromRequest(FrameBufferRequest* request){
	FrameBuffer* frameBuffer = new FrameBuffer();
	frameBuffer->bindAndLockOutputs();
	// Attach de depth buffer
	if(request->isRequiredDepthBuffer()){
		if(request->getDepthBufferRequest()->getInternalFormat() == NULL){
			// El request vino sin formato interno, quiere decir que solo se va a usar para evaluacion de depth
			DepthTexture* depthBuffer = this->getDepthBuffer(request->getDepthBufferRequest());
			this->logTextureReference(depthBuffer);
			depthBuffer->setWriteLock(false);
			frameBuffer->attachDepthBuffer(depthBuffer);
		}
		else{
			// Si el request me viene con formato interno, entonces es que quieren una textura de depth para usarla luego
			ApiTexture* depthTexture = this->getApiTextureForRequest(request->getDepthBufferRequest());
			this->logTextureReference(depthTexture);
			depthTexture->setWriteLock(true);
			frameBuffer->attachDepthBuffer(depthTexture);
		}
	}
	// Consigo y bindeo todas las texturas de salida
	vector<TextureRequest*>* textureRequests = request->getTextureRequests();
	std::vector<TextureRequest*>::iterator it = textureRequests->begin();
	while(it != textureRequests->end()){
		// Se setea el writelock a todas las texturas de salida
		ApiTexture* texture = this->getApiTextureForRequest(*it);
		this->logTextureReference(texture);
		texture->setWriteLock(true);
		frameBuffer->addOutputTexture(texture);
		++it;
	}
	return frameBuffer;
}

FrameBuffer* FrameBufferManager::getUnmanagedFrameBufferAndBind(FrameBufferRequest* request){
	FrameBuffer* frameBuffer = new FrameBuffer();
	if(request->isRequiredDepthBuffer()){
		TextureRequest* depthBufferRequest = request->getDepthBufferRequest();
		if(depthBufferRequest->getInternalFormat() == NULL){
			// El request vino sin formato interno, quiere decir que solo se va a usar para evaluacion de depth
			DepthTexture* depthBuffer = new DepthTexture(vec2(depthBufferRequest->getWidth(), depthBufferRequest->getHeight()));		
			frameBuffer->attachDepthBuffer(depthBuffer);
		}
		else{
			// Si el request me viene con formato interno, entonces es que quieren una textura de depth para usarla luego
			ApiTexture* depthTexture = this->createApiTexture(depthBufferRequest);	
			frameBuffer->attachDepthBuffer(depthTexture);
		}
	}
	// Consigo y bindeo todas las texturas de salida
	vector<TextureRequest*>* textureRequests = request->getTextureRequests();
	std::vector<TextureRequest*>::iterator it = textureRequests->begin();
	while(it != textureRequests->end()){		
		ApiTexture* texture = this->createApiTexture(*it);
		frameBuffer->addOutputTexture(texture);
		++it;
	}
	frameBuffer->bind();
	return frameBuffer;
}

DepthTexture* FrameBufferManager::getDepthBuffer(TextureRequest* request){
	//Busco si existe un depth buffer con el tamaño adecuado y sin usar
	std::list<DepthTexture*>::iterator it = this->avaibleDepthBuffers.begin();
	while(it != this->avaibleDepthBuffers.end()){
		DepthTexture* texture = (*it);
		if((request->matchesDepthTexture(texture)) && (!texture->hasWriteLock())){
			return texture;
		}
		++it;
	}
	//No hay nada, genero un depth buffer nuevo
	DepthTexture* depthTexture = new DepthTexture(vec2(request->getWidth(), request->getHeight()));
    depthTexture->setExternalFormat(request->getFormat());
	this->avaibleDepthBuffers.push_back(depthTexture);
	return depthTexture;
}

ApiTexture* FrameBufferManager::getApiTextureForRequest(TextureRequest* request){
	//Busco si alguna textura ya creada se adapta al request y esta libre para escritura
	std::list<ApiTexture*>::iterator it = this->avaibleTextures.begin();
	while(it != this->avaibleTextures.end()){
		ApiTexture* texture = (*it);
		if((request->matches(texture)) && (!texture->hasWriteLock())){
			return texture;
		}
		++it;
	}
	// Ninguna textura se adapta, creo una nueva
	ApiTexture* texture = this->createApiTexture(request);
	this->avaibleTextures.push_back(texture);	
	return texture;
}

ApiTexture* FrameBufferManager::createApiTexture(TextureRequest* request){	
	vec2 size = vec2(request->getWidth(), request->getHeight());
	ApiTexture* texture = new ApiTexture(size, request->getFormat(), request->getInternalFormat());
	
	// Se aplican los filtros
	std::list<TextureFilterRequest*>::iterator it = request->getFiltersRequests()->begin();
	while(it != request->getFiltersRequests()->end()){
		TextureFilterRequest* filterRequest = (*it);
		// Me fijo si tiene un valor de array (float*), si no tiene le seteo el valor comun (int)
		if(filterRequest->getValueArray() != NULL)
			texture->applyFilter(filterRequest->getType(), filterRequest->getValueArray(), filterRequest->getValueArrayLength());
		else
			texture->applyFilter(filterRequest->getType(), filterRequest->getValue());
		++it;
	}
	return texture;
}

void FrameBufferManager::clearAllFrameBuffers(){
	std::list<FrameBuffer*>::iterator it = this->frameBuffers.begin();
	while(it != this->frameBuffers.end()){
		FrameBuffer* frameBuffer = (*it);
		this->deleteFrameBufferResources(frameBuffer);
		delete frameBuffer;
		it = this->frameBuffers.erase(it);			
	}
}

void FrameBufferManager::deleteFrameBufferResources(FrameBuffer* frameBuffer){
	// Si el depth no tiene mas referencias, lo borro 
	MemoryTexture* depthTexture = frameBuffer->getDepthBuffer();
	if(depthTexture != NULL){
		// Disminuyo una referencia y verifico
		this->textureReferences[depthTexture] -= 1;
		if(this->textureReferences[depthTexture] == 0){
			this->avaibleDepthBuffers.remove((DepthTexture*) depthTexture);
			std::map<MemoryTexture*, unsigned int>::iterator iter = this->textureReferences.find(depthTexture);
			if( iter != this->textureReferences.end() ){
				this->textureReferences.erase( iter );
			}
			delete depthTexture;
		}
	}

	// Para cada textura de output, si no tiene mas referencias, la borro
	vector<ApiTexture*>* outputTextures = frameBuffer->getOutputTextures();
	std::vector<ApiTexture*>::iterator it = outputTextures->begin();
	while(it != outputTextures->end()){
		ApiTexture* texture = (*it);
		//Disminuyo una referencia y verifico
		this->textureReferences[texture] -= 1;
		if(this->textureReferences[texture] == 0){
			std::map<MemoryTexture*, unsigned int>::iterator iter = this->textureReferences.find(texture);
			if( iter != this->textureReferences.end() ){
				this->textureReferences.erase( iter );
			}
			this->avaibleTextures.remove(texture);
			it = outputTextures->erase(it);
			delete texture;
			continue;
		}
		++it;
	}
}


// Logueo que la textura tiene un frame buffer mas asociado
void FrameBufferManager::logTextureReference(MemoryTexture* texture){
	if(this->textureReferences.count(texture) > 0){
		this->textureReferences[texture] += 1;
	}
	else{
		this->textureReferences[texture] = 1;
	}
}

void FrameBufferManager::cleanAllUnusedResorces(){
	// Verifico que frame buffers no se usan hace mas de MAX_UNNUSED_TIME_IN_MILLISECONDS, los borro e intento
	// borrar sus recursos asociados
	std::list<FrameBuffer*>::iterator it = this->frameBuffers.begin();
	while(it != this->frameBuffers.end()){
		FrameBuffer* frameBuffer = (*it);
		if((frameBuffer->getElapsedTimeSinceBinding() > MAX_UNNUSED_TIME_IN_MILLISECONDS)&&(!frameBuffer->hasWriteLock())){
			this->deleteFrameBufferResources(frameBuffer);
			delete frameBuffer;
			it = this->frameBuffers.erase(it);
			continue;
		}
		++it;
	}	
}

void FrameBufferManager::cleanAllNonWriteLockedFrameBuffers(){
	std::list<FrameBuffer*>::iterator it = this->frameBuffers.begin();
	while(it != this->frameBuffers.end()){
		FrameBuffer* frameBuffer = (*it);
		// Si no tiene write lock lo borro
		if(!frameBuffer->hasWriteLock()){
			this->deleteFrameBufferResources(frameBuffer);
			delete frameBuffer;
			it = this->frameBuffers.erase(it);
			continue;
		}
		++it;
	}	
}

void FrameBufferManager::memoryCleanerDaemon(){
	TimeManager* timeManager = TimeManager::getInstance();
	//Si no entre en este metodo en un determinado tiempo (grande ej 1seg), quiere decir que no habia estado renderizando, no borro nada
	if(timeManager->getElapsedTime(this->aplicationConsideredInPauseTimeSlot) > APLICATION_PAUSE_TIME){
		timeManager->updateTimeSlot(this->garbageDaemonSleepTimeSlot);
	}
	timeManager->updateTimeSlot(this->aplicationConsideredInPauseTimeSlot);

	// Si paso el timpo de inactividad del daemon, corro para que se limpie todo lo que no se usa
	if(timeManager->getElapsedTime(this->garbageDaemonSleepTimeSlot) > DAEMON_SLEEP_TIME){
		this->cleanAllUnusedResorces();
		timeManager->updateTimeSlot(this->garbageDaemonSleepTimeSlot);
	}
}



