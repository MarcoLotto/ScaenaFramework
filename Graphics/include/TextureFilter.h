/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

class TextureFilter{

private:
	unsigned int type;
	unsigned int value;

public:
	TextureFilter(unsigned int type, unsigned int value){
		this->type = type;
		this->value = value;
	}

	unsigned int getType(){ return this->type; }
	unsigned int getValue(){ return this->value; }
	void setValue(unsigned int value){ this->value = value; }
};