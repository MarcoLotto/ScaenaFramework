/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIState.h"

class UIPartialTextureState : public UIState{

protected:
	vec2 partialSize;
	vec2 positionIndex;

	virtual void particularUpdate(UIState* finalStateBasicParameters);
	void particularDraw();

public:
	UIPartialTextureState();

	UIState* clone();

	void setPartialSize(vec2 value){ this->partialSize = value;}
	vec2 getPartialSize(){ return this->partialSize;}
	
	virtual void setPositionIndex(vec2 value){ this->positionIndex = value;}
	vec2 getPositionIndex(){ return this->positionIndex;}
};