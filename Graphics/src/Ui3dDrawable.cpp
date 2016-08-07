/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "Ui3dDrawable.h"
#include "UIFactory.h"
#include "FrameBufferManager.h"
#include "GraphicDevice.h"
#include "UIController.h"
#include "TextureFilterRequest.h"

Ui3dDrawable::Ui3dDrawable(Scene* scene, UIController* uiController) : UIElement(uiController){
	this->sceneToDraw = scene;
	this->geometryStage = NULL;

	//Como no es un form, tengo que inicializar a mano
	this->initiate();
}

void Ui3dDrawable::setSceneToDraw(Scene* scene){
	if(this->geometryStage == NULL)
		this->sceneToDraw = scene;
	else
		this->geometryStage->setScene(scene);
}
Scene* Ui3dDrawable::getSceneToDraw(){
	if(this->geometryStage == NULL)
		return this->sceneToDraw;
	else
		return this->geometryStage->getScene();
}

//Incializacion de los estados del elemento 
void Ui3dDrawable::initiateStates(){		
	this->basicState = UIFactory::createState(vec2(0.0f), 0.0f, vec2(1.0f), 1.0f, NULL);		
}
//Inicializacion de los subelementos
void Ui3dDrawable::initiateElements(){
	this->internalState->makeStateTransition(this->basicState);	
}

void Ui3dDrawable::bindElementsToStates(){
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void Ui3dDrawable::handleEvents(){	
}

vec2 Ui3dDrawable::getDrawableAreaSize(){
	return this->externalState->getActualState()->getScale() * this->uiController->getScreenSize();
}

// Consigue un framebuffer que se adapte a lo necesitado y lo bindea
void Ui3dDrawable::manageAndBindFrameBuffer(){
	this->geometryStage->render(false);
}

void Ui3dDrawable::drawScene(){
	//Consigo y bindeo un frame buffer (me guardo que frame buffer esta activo ahora)
	FrameBuffer* currentFrameBuffer = FrameBufferManager::getInstance()->getActiveFrameBuffer();

	// Si no existe, creamos el stage para el renderizado
	if(this->geometryStage == NULL){
		vec2 drawSize = this->getDrawableAreaSize();
		GraphicDeviceConstantCatalog* catalog = GraphicDevice::getInstance()->getConstantCatalog();
		this->geometryStage = new ForwardGeometryStage(drawSize.x, drawSize.y, false, catalog->getFormatRGBA(), catalog->getFormatRGBA());  // REVIEW: Mismo formato interno que externo? Defaultea a algo? A que?
		this->geometryStage->setScene(this->sceneToDraw);
	}
	else{
		this->geometryStage->getBackBuffer()->getOutputTexture()->setWriteLock(false);
	}	
	// Renderizamos la escena y la guardamos en un framebuffer
	GraphicDevice::getInstance()->enableMode(GraphicDevice::getInstance()->getConstantCatalog()->getDepthTestFlag());
	this->geometryStage->render(false);
	GraphicDevice::getInstance()->disableMode(GraphicDevice::getInstance()->getConstantCatalog()->getDepthTestFlag());
	this->geometryStage->updateOutputData();

	// Vamos a lockear la textura para que no me la borre el frame buffer manager
	this->geometryStage->getBackBuffer()->getOutputTexture()->setWriteLock(true);
	
	// Volvemos al framebuffer original
	currentFrameBuffer->bind();
	vec2 screenSize = this->uiController->getScreenSize();
	GraphicDevice::getInstance()->setViewport(0,0, screenSize.x, screenSize.y);

	//Seteo como textura a la salida del frame buffer
	this->internalState->getActualState()->setTexture(this->geometryStage->getBackBuffer()->getOutputTexture());
}


void Ui3dDrawable::draw(){	
	// Actualizamos y generamos la escena a dibujar
	drawScene();	

	// Dibujamos la imagen generada en pantalla
	this->internalState->draw();
}

void Ui3dDrawable::update(UIState* externalState){	
	//Mergeo los estados
	UIState finalState = this->mergeParameters(externalState);	

	//Actualizo el estado externo
	this->externalState->update(&finalState);	
	UIState finalStateBasicParameters = this->externalState->getActualState()->mergeBasicParametersWithoutParentScale(externalState);
	
	// Ahora si actualizo el estado interno
	this->internalState->update(&finalStateBasicParameters);	
}

Ui3dDrawable::~Ui3dDrawable(){	
	if(this->basicState != NULL){
		delete this->basicState;
		this->basicState = NULL;
	}
	if(this->geometryStage != NULL){
		// Primero liberamos la textura reservada al frame buffer manager y despues borrarmos el stage
		if(this->geometryStage->getBackBuffer() != NULL && this->geometryStage->getBackBuffer()->getOutputTexture() != NULL){
			this->geometryStage->getBackBuffer()->getOutputTexture()->setWriteLock(false);
		}
		delete this->geometryStage;
	}
}
