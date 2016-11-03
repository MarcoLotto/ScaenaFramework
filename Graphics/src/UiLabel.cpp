/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UiLabel.h"
#include "UIFactory.h"
#include "FrameBufferManager.h"
#include "GraphicDevice.h"
#include "UIController.h"
#include "TextureFilterRequest.h"
#include "WindowConnector.h"

#define DEFAULT_LETTER_SEPARATION_FACTOR 2.0f
#define DEFAULT_CHARSPERLINE 12
#define TOTAL_REPRESENTABLE_CHARACTERS 96

#define LETTER_HEIGHT_IN_PIXELS 32
#define LETTER_WIDTH_IN_PIXELS 50

#define LETTER_X_RELATIVE_SIZE 0.20f
#define LETTER_Y_RELATIVE_SIZE 0.35f

#define FIRST_REPRESENTABLE_CHARACTER 32

UiLabel::UiLabel(const char* filename, UIController* uiController) : UIElement(uiController){
	//La ruta de la imagen a cargar
	this->filename = filename;
	this->setCharactersPerLine(DEFAULT_CHARSPERLINE);
	this->setLetterSeparationFactor(DEFAULT_LETTER_SEPARATION_FACTOR);
	this->frameBuffer = NULL;
	this->textHasChanged = false;
	this->letterHeightFactor = 1.0f;

	//Como no es un form, tengo que inicializar a mano
	this->initiate();
}

//Incializacion de los estados del elemento 
void UiLabel::initiateStates(){
	float verticalFactor = 1.0f / (TOTAL_REPRESENTABLE_CHARACTERS/(float)DEFAULT_CHARSPERLINE);
	float horizontalFactor = 1.0f/(float)DEFAULT_CHARSPERLINE;
	this->partialLabelState = UIFactory::createPartialTextureState(vec2(0.0f), 0.0f, vec2(1.0f, 1.0f), 1.0f, 
		this->filename, vec2(horizontalFactor, verticalFactor), vec2(1.0f));	
	this->finalLabelState = UIFactory::createState(vec2(0.0f), 0.0f, vec2(0.0f, LETTER_Y_RELATIVE_SIZE), 1.0f, this->filename);			
}
//Inicializacion de los subelementos
void UiLabel::initiateElements(){
	this->internalState->makeStateTransition(this->finalLabelState);	
}

void UiLabel::bindElementsToStates(){
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void UiLabel::handleEvents(){	
}

vec2 UiLabel::calculateLabelSizeInPixels(vec2 pixelsPerLetter){
	vec2 size( pixelsPerLetter.x * (this->text.length()+1), pixelsPerLetter.y);
    
    // Ajustamos el tama–o de la textura final para que quede como potencia de 2
    if(size.x <= 2){ size.x = 2; }
    else if(size.x <= 5){ size.x = 4; }
    else if(size.x <= 10){ size.x = 8; }
    else if(size.x <= 23){ size.x = 16; }
    else if(size.x <= 48){ size.x = 32; }
    else if(size.x <= 90){ size.x = 64; }
    else if(size.x <= 175){ size.x = 128; }
    else if(size.x <= 350){ size.x = 256; }
    else if(size.x <= 700){ size.x = 512; }
    else{ size.x = 1024; }
    return size;
}

// Consigue un framebuffer que se adapte a lo necesitado y lo bindea
void UiLabel::manageAndBindFrameBuffer(vec2 letterSizeInPixels){
	//Genero el request para la nueva textura y frame buffer
	GraphicDeviceConstantCatalog* catalog = GraphicDevice::getInstance()->getConstantCatalog();
	FrameBufferRequest* fboRequest = new FrameBufferRequest();
	vec2 size = this->calculateLabelSizeInPixels(letterSizeInPixels);
	TextureRequest* textureRequest = new TextureRequest(size.x, size.y, catalog->getFormatRGBA(), catalog->getFormatRGBA());
	textureRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMinFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	textureRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMagFilterFlag(), catalog->getTextureLinearFilteringFlag()));
	fboRequest->addTextureRequest(textureRequest);

	// Libero la textura anterior
	if(this->frameBuffer != NULL){
		this->frameBuffer->bind();  //Esto es para que el recolector de basura del frame buffer manager no trate de borarlo antes de comprobar si este anda
		this->frameBuffer->getOutputTextures()->at(0)->setWriteLock(false);
		// Si cambio el tamaño de la textura del frame buffer, fuerzo a que me de otra nueva
		if(this->frameBuffer->getOutputTextures()->at(0)->getSize() != size){
			this->frameBuffer = NULL;
		}
	}
	this->frameBuffer = FrameBufferManager::getInstance()->getFrameBufferAndBind(fboRequest, this->frameBuffer);
	GraphicDevice::getInstance()->setViewport(0,0, size.x, size.y);
}

// Consigue el tamaño relativo a la pantalla de la letra
vec2 UiLabel::getRelativeLetterSize(){
	return vec2(1.0f / (float) this->text.length(), 1.0f);	
}

// Consigue el tamaño en pixels de la letra
vec2 UiLabel::getLetterSizeInPixels(){
	return vec2(LETTER_WIDTH_IN_PIXELS, LETTER_HEIGHT_IN_PIXELS);
}

// Genera el label en memoria (para dibujar en menos batch)
void UiLabel::generateLabel(){
	// Trabajo con el estado de label parcial (por letras)
	vec2 unmodifiedPosition = this->partialLabelState->getPosition();
	
	// Calculo la separacion entre letras y tamaño final
	vec2 externalScale = this->externalState->getActualState()->getScale();
	vec2 letterRelativeSize = this->getRelativeLetterSize();
	vec2 startPosition = -vec2(letterRelativeSize.x / (float)this->letterSeparationFactor, 0.0f);
	this->partialLabelState->setPosition(startPosition);
	this->partialLabelState->setScale(vec2(letterRelativeSize.x, letterRelativeSize.y * this->letterHeightFactor));
	//Consigo y bindeo un frame buffer para guardar el label (me guardo que frame buffer esta activo ahora)
	FrameBuffer* currentFrameBuffer = FrameBufferManager::getInstance()->getActiveFrameBuffer();
	this->manageAndBindFrameBuffer(this->getLetterSizeInPixels());	
	
	GraphicDevice::getInstance()->clearColorBuffer();
	GraphicDeviceConstantCatalog* apiConstants = GraphicDevice::getInstance()->getConstantCatalog();
	GraphicDevice::getInstance()->disableMode(apiConstants->getDepthTestFlag());	
	
	//Recorro el texto y por cada letra actualizo el estado interno y dibujo
	for(unsigned int i=0; i<this->text.length(); i++){
		//Avanzo una posicion (se divide por el separation factor por si se quiere letras mas juntas o separadas)
		vec2 positionIncrement = vec2(letterRelativeSize.x / (float)this->letterSeparationFactor, 0.0f);
		this->partialLabelState->setPosition(this->partialLabelState->getPosition() + positionIncrement);
		//Apunto a la letra correcta
		char letter = this->text.at(i);
		if(letter < FIRST_REPRESENTABLE_CHARACTER) letter = FIRST_REPRESENTABLE_CHARACTER; // Evito caracteres no representables
		letter -= (FIRST_REPRESENTABLE_CHARACTER);
		this->partialLabelState->setPositionIndex(vec2((letter % this->charactersPerLine), -glm::floor(letter / (float) this->charactersPerLine) - 1));

		//Mergeo las propiedades basicas de los estados externos y mando a dibujar. (Es necesario primero hacerle update al estado, recordar que esto solo se hace cuando hay cambios)
		UIState emptyState;
		this->partialLabelState->update(&emptyState);
		this->partialLabelState->draw();
	}
	//Vuelvo a setear la posicion original
	this->partialLabelState->setPosition(unmodifiedPosition);
	
	//Vuelvo al frame buffer que habia antes y al viewport de la pantalla
	currentFrameBuffer->bind();
	vec2 screenSize = this->uiController->getScreenSize();
	GraphicDevice::getInstance()->setViewport(0,0, screenSize.x, screenSize.y);

	//Seteo como textura a la salida del frame buffer
	this->finalLabelState->setTexture(this->frameBuffer->getOutputTextures()->at(0));
}


void UiLabel::draw(){	
	// Verifico si tengo que generar el label en memoria o ya lo tengo generado
	if(this->textHasChanged || this->frameBuffer==NULL){		
		this->generateLabel();
		this->textHasChanged = false;
	}
	// Mando a dibujar el label completo (en un solo batch)
	this->finalLabelState->draw();
}

Texture* UiLabel::getLabelTexture(){
	if(this->textHasChanged || this->frameBuffer==NULL){		
		this->generateLabel();
	}
	return this->finalLabelState->getTexture();
}

void UiLabel::update(UIState* externalState){	
	//Mergeo los estados
	UIState finalState = this->mergeParameters(externalState);

	//Actualizo el estado externo
	this->externalState->update(&finalState);	

	//Calculo el tamaño de label, mergeo las propiedades basicas de los estados externos y mando a dibujar
	UIState finalStateBasicParameters = this->externalState->getActualState()->mergeBasicParametersWithoutParentScale(externalState);

	//Voy a tomar como escala la escala mas chica (para que no se deforme el label)
	vec2 scale = this->getLowerScaleComponent(finalStateBasicParameters.getScale());
	finalStateBasicParameters.setScale(scale);	

	// Ahora si actualizo el estado interno
	this->finalLabelState->update(&finalStateBasicParameters);	
}

vec2 UiLabel::getLowerScaleComponent(vec2 scale){
	if(scale.x < scale.y)
		return vec2(scale.x);
	return vec2(scale.y);
}

void UiLabel::setText(string text){ 
	this->text = text; 
	this->textHasChanged=true;
	// Como cambio la cantidad de letras tuvo que haber cambiado el tamaño del label
	float newXScale = this->text.length() * LETTER_X_RELATIVE_SIZE;
	float newYScale = LETTER_Y_RELATIVE_SIZE;
	this->finalLabelState->setScale(vec2(newXScale, newYScale));
}

UiLabel::~UiLabel(){
	if(this->partialLabelState != NULL){
		delete this->partialLabelState;
		this->partialLabelState = NULL;
	}
	if(this->finalLabelState != NULL){
		delete this->finalLabelState;
		this->finalLabelState = NULL;
	}
}
