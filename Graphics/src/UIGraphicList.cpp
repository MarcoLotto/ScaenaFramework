/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UIGraphicList.h"

#define DEFAULT_FONT "./Textures/UI/font2.png"
#define EMPTY_BUTTONS_BACKGROUND "./Textures/UI/ListButton1.png"
#define DEFAULT_ITEMS_IMAGE "./Textures/UI/transparent.png"

UIGraphicList::UIGraphicList(UIController* uiController, vec2 scrollBarSize, int scrollBarOptionsCount, string upButtonTexture, 
							 string downButtonTexture, string backgroundTexture, vec2 labelPos, float labelSize, vec2 imgPos, vec2 imgSize) : UIElement(uiController){
	this->graphicListInitialize(uiController, scrollBarSize, scrollBarOptionsCount, upButtonTexture, downButtonTexture, backgroundTexture, 
		labelPos, labelSize, imgPos, imgSize);
}

UIGraphicList::UIGraphicList(UIController* uiController, vec2 scrollBarSize, int scrollBarOptionsCount, string upButtonTexture, 
							 string downButtonTexture, string backgroundTexture, vec2 imgPos, vec2 imgSize) : UIElement(uiController){
	this->graphicListInitialize(uiController, scrollBarSize, scrollBarOptionsCount, upButtonTexture, downButtonTexture, backgroundTexture, 
		vec2(0.0f), 0.0f, imgPos, imgSize);
}

void UIGraphicList::graphicListInitialize(UIController* uiController, vec2 scrollBarSize, int scrollBarOptionsCount, string upButtonTexture, string downButtonTexture, 
		string backgroundTexture, vec2 labelPos, float labelSize, vec2 imgPos, vec2 imgSize){
	this->scrollBarOptionsCount = scrollBarOptionsCount;
	this->upButtonTexture = upButtonTexture;
	this->downButtonTexture = downButtonTexture;
	this->backgroundTexture = backgroundTexture;
	this->scrollBarSize = scrollBarSize;
	this->selectedItemPosition = -1;
	this->firstItemInListPosition = 0;
	this->labelPos = labelPos;
	this->imgPos = imgPos;
	this->imgSize = imgSize;
	this->labelSize = labelSize;
	this->usingLabels = (labelSize > 0.0f);

	//Como no es un form, tengo que inicializar a mano
	this->initiate();
}

//Incializacion de los estados del elemento 
void UIGraphicList::initiateStates(){
	this->basicState = UIFactory::createPartialTextureState(vec2(0.0f), 0.0f, this->scrollBarSize, 1.0f, this->backgroundTexture.c_str(), vec2(1.0f/3.0f, 1.0f), vec2(0.0f));		
}

//Inicializacion de los subelementos
void UIGraphicList::initiateElements(){	
	float optionSizeY = this->scrollBarSize.y / (2.0f + this->scrollBarOptionsCount);
	vec2 actualPosition = vec2(0.0f, 0.0f);
	this->upButton = UIFactory::createButton(actualPosition, 0.0f, vec2(this->scrollBarSize.x, optionSizeY), "", 0.0f, vec2(0.01f, 0.02f), this->upButtonTexture.c_str(), DEFAULT_FONT, this->uiController);
	for(unsigned int i=0; i < this->scrollBarOptionsCount; i++){
		actualPosition.y += optionSizeY;
		// Para ahorrar performance, si no usamos labels en los items, creamos botones sin labels
		if(this->usingLabels){
			this->listButtons.push_back(UIFactory::createImageAndLabelButton(actualPosition, 0.0f, vec2(this->scrollBarSize.x, optionSizeY), "", this->labelSize, this->labelPos, EMPTY_BUTTONS_BACKGROUND, DEFAULT_FONT, DEFAULT_ITEMS_IMAGE, this->imgPos, this->imgSize, this->uiController));  
		}
		else{
			this->listButtons.push_back(UIFactory::createImageButton(actualPosition, 0.0f, vec2(this->scrollBarSize.x, optionSizeY), EMPTY_BUTTONS_BACKGROUND, DEFAULT_ITEMS_IMAGE, this->imgPos, this->imgSize, this->uiController));  
		}
	}
	actualPosition.y += optionSizeY;
	this->downButton = UIFactory::createButton(actualPosition, 0.0f, vec2(this->scrollBarSize.x, optionSizeY), "", 0.0f, vec2(0.01f, 0.02f), this->downButtonTexture.c_str(), DEFAULT_FONT, this->uiController);
}

void UIGraphicList::bindElementsToStates(){
	this->basicState->addElement(this->upButton);	
	for(unsigned int i=0; i < this->scrollBarOptionsCount; i++){
		this->basicState->addElement(this->listButtons[i]);
	}
	this->basicState->addElement(this->downButton);

	this->internalState->makeStateTransition(this->basicState);	
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void UIGraphicList::handleEvents(){
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == ONFOCUSHOLD_UIEVENTCODE) return;
		
	//Compruebo de que elemento se trata el evento y lo proceso
	if(this->onFocusEvent.getStateCode() == ONFOCUSRELEASE_UIEVENTCODE){
		if(this->onFocusEvent.getAfectedElement() == this->upButton){
			this->processUpScrollButtonEvent();		
		}
		else if(this->onFocusEvent.getAfectedElement() == this->downButton){
			this->processDownScrollButtonEvent();		
		}
		else{
			for(unsigned int i=0; i < this->listButtons.size(); i++){
				if(this->onFocusEvent.getAfectedElement() == this->listButtons[i]){
					this->processOptionButtonEvent(i);		
				}
			}
		}
	}
	//Vacio el evento (No me interesa en este caso mantener estados de hold ni nada de eso)
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

void UIGraphicList::processUpScrollButtonEvent(){
	if(this->firstItemInListPosition > 0){
		this->firstItemInListPosition--;		
		this->updateGraphicList();		
	}
	this->selectedItemPosition = -1;
}
void UIGraphicList::processDownScrollButtonEvent(){
	if(this->firstItemInListPosition < (int) (this->listItems.size() - this->listButtons.size())){
		this->firstItemInListPosition++;		
		this->updateGraphicList();		
	}	
	this->selectedItemPosition = -1;
}
void UIGraphicList::processOptionButtonEvent(int position){
	int positionOnItemList = this->firstItemInListPosition + position;
	if(positionOnItemList < this->listItems.size()){
		this->selectedItemPosition = positionOnItemList;
	}
	else{
		this->selectedItemPosition = -1;
	}
}

void UIGraphicList::setListItems(vector<UIGraphicListItem> items){ 
	this->clear();
	this->listItems = items; 	
	this->updateGraphicList();
}

UIGraphicList::~UIGraphicList(){
	if(this->basicState != NULL){
		delete this->basicState;
		this->basicState = NULL;
	}
	if(this->upButton != NULL){
		delete this->upButton;
		this->upButton = NULL;
	}
	if(this->downButton != NULL){
		delete this->downButton;
		this->downButton = NULL;
	}
	for(unsigned int i=0; i < this->listButtons.size(); i++){
		if(this->listButtons[i] != NULL){
			delete this->listButtons[i];
		}
	}
	this->listButtons.clear();
}

void UIGraphicList::updateGraphicList(){
	for(unsigned int i=0; i < this->listButtons.size(); i++){		
		if(this->firstItemInListPosition + i < this->listItems.size()){			
			UIGraphicListItem* item = &this->listItems[this->firstItemInListPosition + i]; 

			// Actualizamos el label y la textura del boton
			UiButton* itemButton = this->listButtons[i];
			if(itemButton->getLabel() != NULL){
				itemButton->getLabel()->setText(item->getItemLabel());
			}
			if(itemButton->getImage() != NULL && item->getItemTexture() != NULL){
				itemButton->getImage()->setTexture(item->getItemTexture()->clone());
			}
		}
	}
}

void UIGraphicList::clear(){	
	// Limpiamos los botones
	for(unsigned int i=0; i < this->listButtons.size(); i++){		
		UiButton* itemButton = this->listButtons[i];
		if(itemButton->getLabel() != NULL){
			itemButton->getLabel()->setText("");
		}
		itemButton->getImage()->setTexture(new FileTexture(DEFAULT_ITEMS_IMAGE));
	}
	// Volvemos a inicializar las referencias de posicion de la lista
	this->firstItemInListPosition = 0; 
	this->selectedItemPosition = -1;

	// Borramos las texturas de los items anteriores y limpiamos la lista
	for(unsigned int i=0; i < this->listItems.size(); i++){
		Texture* itemTexture = this->listItems[i].getItemTexture();
		if(itemTexture != NULL){
			delete itemTexture;
		}
	}
	this->listItems.clear();
}