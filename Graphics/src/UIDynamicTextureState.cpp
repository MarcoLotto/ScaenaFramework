/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UIDynamicTextureState.h"
#include "TimeManager.h"

UIDynamicTextureState::UIDynamicTextureState(){
	this->shader = UIShader::getInstance();
	this->referenceToOriginal = NULL;
	this->initBasicParameters();

	this->setAnimationLoop(false);
	this->setAnimationTime(0);
	this->setRowsToIterate(0);
	this->timeSlot = TimeManager::getInstance()->reservTimeSlot();
}

void UIDynamicTextureState::particularUpdate(UIState* finalStateBasicParameters){
	int frameAnimationTime = (int)((this->getPartialSize().x * this->animationTime) / (float)this->rowsToIterate);
	//Verifico si el tiempo de un casillero paso.
	TimeManager* timeManager = TimeManager::getInstance();
	if(frameAnimationTime <= timeManager->getElapsedTime(this->timeSlot)){
		//Hago update del time slot
		timeManager->updateTimeSlot(this->timeSlot);

		//Modifico la posicion en la imagen
		this->positionIndex.x++;
		if(this->positionIndex.x*this->partialSize.x >= 1.0f){
			this->positionIndex.x = 0;
			this->positionIndex.y++;
		}
		//Verifico si llegue al final de la animacion
		if(this->positionIndex.y >= this->originalPosIndex.y + this->rowsToIterate){
			this->positionIndex = this->originalPosIndex;
		}
	}
	// Mando a generar la matriz de transfomacion
	this->generateTransformationMatrix(finalStateBasicParameters);
}

void UIDynamicTextureState::setPositionIndex(vec2 value){ 
	this->positionIndex = value;
	this->originalPosIndex = value;
}

UIState* UIDynamicTextureState::clone(){
	UIDynamicTextureState* cloned = new UIDynamicTextureState();
	//Seteo todos lo atributos propios del estado
	cloned->setPosition(this->getPosition());
	cloned->setRotation(this->getRotation());
	cloned->setScale(this->getScale());
	cloned->setTransparency(this->getTransparency());
	if(this->getTexture() != NULL){
		cloned->setTexture(this->getTexture()->clone(), true);
	}
	cloned->setPositionIndex(this->getPositionIndex());
	cloned->setPartialSize(this->getPartialSize());
	cloned->setAnimationLoop(this->isAnimationLoop());
	cloned->setAnimationTime(this->getAnimationTime());
	cloned->setRowsToIterate(this->getRowsToIterate());
	cloned->setDestroyedAfterTransition(this->isDestroyedAfterTransition());

	//Seteo la referencia al estado original (no copia)
	if(this->referenceToOriginal == NULL){
		cloned->setReferenceToOriginal(this);
	}
	else{
		cloned->setReferenceToOriginal(this->getReferenceToOriginal());
	}

	//Agrego los UIElements que deben dibujar en este estado
	std::list<UIElement*>::iterator it = this->getDrawingElements()->begin();
	while(it != this->getDrawingElements()->end()){
		cloned->addElement(*it);
		++it;
	}
	return cloned;
}
