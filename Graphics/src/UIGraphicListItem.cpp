/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UIGraphicListItem.h"

UIGraphicListItem::UIGraphicListItem(){ 
	this->itemLabel = ""; 
	this->itemTexture = NULL; 
	this->id = -1; 
}

UIGraphicListItem::UIGraphicListItem(string itemLabel, Texture* itemTexture, int id){ 
	this->itemLabel = itemLabel; 
	this->itemTexture = itemTexture; 
	this->id = id; 
}

UIGraphicListItem::~UIGraphicListItem(){
}

void UIGraphicListItem::setItemTexture(Texture* texture){ 	
	this->itemTexture = texture; 
}