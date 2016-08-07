/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Texture.h"
#include "FileTexture.h"
#include <string>
using namespace std;

class UIGraphicListItem{

private:
	Texture* itemTexture;  // Imagen que se mostrara en el item de la lista
	string itemLabel;      // Label que se mostrara en el item de la lista
	int id;				   // Permite guardar como dato extra un id para el elemento

public:
	UIGraphicListItem();
	UIGraphicListItem(string itemLabel, Texture* itemTexture, int id=-1);
	virtual ~UIGraphicListItem();

	void setItemTexture(Texture* texture);
	void setItemLabel(string itemLabel){ this->itemLabel = itemLabel; }

	Texture* getItemTexture(){ return this->itemTexture; }
	string getItemLabel(){ return this->itemLabel; }

	void setId(int id){ this->id = id; }
	int getId(){ return this->id; }
};