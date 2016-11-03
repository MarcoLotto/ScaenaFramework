/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UIState.h"
#include "UIElement.h"

UIState::UIState(){
	this->shader = UIShader::getInstance();
	this->referenceToOriginal = NULL;
	this->destroyedAfterTransition = false;
	this->initBasicParameters();
}

UIState* UIState::clone(){
	UIState* cloned = new UIState();
	//Seteo todos lo atributos propios del estado
	cloned->setPosition(this->getPosition());
	cloned->setRotation(this->getRotation());
	cloned->setScale(this->getScale());
	cloned->setTransparency(this->getTransparency());
	if(this->getTexture() != NULL){
		cloned->setTexture(this->getTexture()->clone(), true);
	}
	cloned->setDestroyedAfterTransition(this->isDestroyedAfterTransition());
	
	//Seteo la referencia al estado original (no copia)
	if(this->referenceToOriginal == NULL){
		cloned->setReferenceToOriginal(this);
	}
	else{
		cloned->setReferenceToOriginal(this->getReferenceToOriginal());
	}

	//Agrego los UIElements que deben dibujar en este estado
	std::list<UIElement*>::iterator it = this->drawingElements.begin();
	while(it != this->drawingElements.end()){
		cloned->addElement(*it);
		++it;
	}
	return cloned;
}

//Interpola los atributos del estado, con un estado final y guarda en estado destination
void UIState::interpolate(UIState* finalState, UIState* destination, float interval){
	//Interpolo todos los atributos del state
	destination->setPosition(this->interpolateParameter(this->getPosition(), finalState->getPosition(), interval));
	destination->setRotation(this->interpolateParameter(this->getRotation(), finalState->getRotation(), interval));
	destination->setScale(this->interpolateParameter(this->getScale(), finalState->getScale(), interval));
	
	destination->setTransparency(this->interpolateParameter(this->getTransparency(), finalState->getTransparency(), interval));
	destination->setTexture(this->texture);	
}

//Evalua si se realizo click en algun elemento de este estado
UIEventHandler UIState::evaluateClick(vec2 coords, int state, UIState* parentState){
	//Mergeo los parametros
	UIState finalState = this->mergeBasicParameters(parentState);

	//Calculo los parametros actuales
	vec2 fixedPosition = finalState.getPosition();
	mat4 rotMat = glm::translate(mat4(1.0f), vec3(fixedPosition, 0.0f));
	rotMat = glm::rotate(rotMat, -finalState.getRotation(), vec3(0.0f, 0.0f, 1.0f));
	rotMat = glm::translate(rotMat, vec3(-fixedPosition, 0.0f));
	
	vec2 fixedTotalSize = finalState.getScale() + fixedPosition;
	vec2 modifiedCoords = vec2(rotMat * vec4(coords, 0.0f, 1.0f));  //Para comprobar matcheo con este form
	
	//Verifico si el click fue realizado dentro del elemento
	if((modifiedCoords.x >= fixedPosition.x)&&(modifiedCoords.x <= fixedTotalSize.x)){
		if((modifiedCoords.y >= fixedPosition.y)&&(modifiedCoords.y <= fixedTotalSize.y)){
			//Calculo donde caen las coordenadas para el hijo
			mat4 rotMatForChilds = glm::translate(mat4(1.0f), vec3(-fixedPosition, 0.0f));
			vec2 modifiedCoordsForChilds = vec2(rotMatForChilds * vec4(coords, 0.0f, 1.0f));
			rotMatForChilds = glm::rotate(mat4(1.0f), -finalState.getRotation(), vec3(0.0f, 0.0f, 1.0f));
			modifiedCoordsForChilds = vec2(rotMatForChilds * vec4(modifiedCoordsForChilds, 0.0f, 1.0f));
			//Voy a buscar en que hijo se clickeo y devuelvo el evento
			return this->searchClickOnChilds(modifiedCoordsForChilds, state);		
		}
	}
	UIEventHandler emptyHandler(NULL, NOEVENT_UIEVENTCODE, coords);
	return emptyHandler;
}	

UIEventHandler UIState::searchClickOnChilds(vec2 coords, int state){
	//Recorro los UIElementos que se dibujan en pantalla, en orden inverso, buscando clicks
	std::list<UIElement*>::reverse_iterator it = this->drawingElements.rbegin();
	while(it != this->drawingElements.rend()){
		UIEventHandler eventHandler = (*it)->onClick(coords, state);
		if(eventHandler.getStateCode() != NOEVENT_UIEVENTCODE){
			return eventHandler;
		}
		++it;
	}
	//No se clickeo ningun hijo, pero marco como que hubo un foco (el mismo)
	UIEventHandler nullEvent(NULL, ONFOCUS_UIEVENTCODE, coords);
	return nullEvent;
}

void UIState::particularUpdate(UIState* finalStateBasicParameters){
	this->generateTransformationMatrix(finalStateBasicParameters);
}

void UIState::initBasicParameters(){
	this->setPosition(vec2(0.0f));
	this->setRotation(0.0f);
	this->setScale(vec2(1.0f));
	this->setTransparency(1.0f);
	this->texture = NULL;
	this->deleteTextureOnStateDelete = false;
	this->setTexture(NULL);
	this->transparencyToDraw = this->transparency;
}

UIState UIState::mergeBasicParameters(UIState* state){
	UIState mergedState;
	mat4 rotMat = glm::translate(mat4(1.0f), vec3(state->getPosition(), 0.0f));
	rotMat = glm::rotate(rotMat, state->getRotation(), vec3(0.0f, 0.0f, 1.0f));
	rotMat = glm::translate(rotMat, vec3(-state->getPosition(), 0.0f));

	//Solo se mergean propiedades basicas, no imagenes ni nada de eso
	mergedState.setPosition(vec2(rotMat * vec4(this->position + state->getPosition(), 0.0f, 1.0f)));
	mergedState.setRotation(this->rotation + state->getRotation());
	mergedState.setScale(this->scale * state->getScale());
	this->transparencyToDraw = this->transparency * state->getTransparency();
	mergedState.setTransparency(this->transparencyToDraw);
    
	return mergedState;
}

UIState UIState::mergeBasicParametersWithoutParentScale(UIState* state){
	UIState mergedState = this->mergeBasicParameters(state);
	mergedState.setScale(this->getScale());
	return mergedState;
}

UIState UIState::mergeBasicParametersWithParentScale(UIState* state){
	UIState mergedState;
	mat4 rotMat = glm::translate(mat4(1.0f), vec3(state->getPosition(), 0.0f));
	rotMat = glm::rotate(rotMat, state->getRotation(), vec3(0.0f, 0.0f, 1.0f));
	rotMat = glm::translate(rotMat, vec3(-state->getPosition(), 0.0f));

	//Solo se mergean propiedades basicas, no imagenes ni nada de eso
	mergedState.setPosition(vec2(rotMat * vec4(state->getScale() * this->position + state->getPosition(), 0.0f, 1.0f)));
	mergedState.setRotation(this->rotation + state->getRotation());
	mergedState.setScale(this->scale * state->getScale());
	this->transparencyToDraw = this->transparency * state->getTransparency();
	mergedState.setTransparency(this->transparencyToDraw);
    
	return mergedState;
}

void UIState::particularDraw(){
	this->shader->draw(this->texture, vec2(1.0f), vec2(0.0f), this->transformations, this->transparencyToDraw); 
}

void UIState::draw(){
	//Primero dibujo la textura del estado actual
	if(this->texture){
		this->particularDraw();
	}
	//Dibujo los UIElements que deben dibujar en este estado
	std::list<UIElement*>::iterator it = this->drawingElements.begin();
	while(it != this->drawingElements.end()){
		(*it)->draw();
		++it;
	}
}

void UIState::update(UIState* externalState){
	//Mergeo los estados para conseguir los parametros basicos interpolados
	UIState finalStateBasicParameters = this->mergeBasicParameters(externalState);

	//Primero hago el update particular de este estado
	this->particularUpdate(&finalStateBasicParameters);

	//Update de los UIElements de este estado
	std::list<UIElement*>::iterator it = this->drawingElements.begin();
	while(it != this->drawingElements.end()){
		(*it)->update(&finalStateBasicParameters);
		++it;
	}
}

UIState::~UIState(){
	// Si tenemos textura asociada y esta indicado que la borremos, la borramos
	if(this->texture != NULL && this->deleteTextureOnStateDelete){
		delete this->texture;
		this->texture = NULL;
	}
}

float UIState::interpolateParameter(float vIni, float vFin, float interval){
	return (1.0f - interval) * vIni + interval * vFin;
}
vec2 UIState::interpolateParameter(vec2 vIni, vec2 vFin, float interval){
	vec2 result;
	result.x = this->interpolateParameter(vIni.x, vFin.x, interval);
	result.y = this->interpolateParameter(vIni.y, vFin.y, interval);
	return result;
}

// Actualiza la matriz de transformacion del la escena
void UIState::generateTransformationMatrix(UIState* finalStateBasicParameters){
	vec2 pos = finalStateBasicParameters->getPosition() - vec2(0.5f);
	float rot = finalStateBasicParameters->getRotation();
	vec2 scale = finalStateBasicParameters->getScale();
	
	//Calculo la matriz de modelo	
	mat4 scaleMat = glm::scale(mat4(1.0f), vec3(scale.x, scale.y, 1.0f));
	mat4 rotMat = glm::rotate(mat4(1.0f), rot, vec3(0.0f, 0.0f, 1.0f));
	mat4 transMat = glm::translate(mat4(1.0f), vec3(pos.x, pos.y, 0.0f));
	this->transformations = transMat * rotMat * scaleMat; 
}

void UIState::setTexture(Texture* texture, bool deleteTextureOnStateDelete){ 
	this->texture = texture;
	this->deleteTextureOnStateDelete = deleteTextureOnStateDelete;
}

void UIState::setTextureAndDeletePrevious(Texture* texture, bool deleteTextureOnStateDelete){ 
	if(this->texture != texture && this->texture != NULL){
		delete this->texture;
	}
	this->setTexture(texture, deleteTextureOnStateDelete);
}
