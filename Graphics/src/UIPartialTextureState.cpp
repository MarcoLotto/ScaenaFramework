/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UIPartialTextureState.h"

UIPartialTextureState::UIPartialTextureState(){
	this->shader = UIShader::getInstance();
	this->referenceToOriginal = NULL;
	this->initBasicParameters();
}

void UIPartialTextureState::particularUpdate(UIState* finalStateBasicParameters){
	this->generateTransformationMatrix(finalStateBasicParameters);
}

void UIPartialTextureState::particularDraw(){
	this->shader->draw(this->texture, this->partialSize, this->positionIndex, this->transformations, this->transparencyToDraw); 
}

UIState* UIPartialTextureState::clone(){
	UIPartialTextureState* cloned = new UIPartialTextureState();
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

