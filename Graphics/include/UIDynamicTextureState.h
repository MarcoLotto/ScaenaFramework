/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIPartialTextureState.h"

class UIDynamicTextureState :  public UIPartialTextureState{

private:
	int timeSlot;
	int rowsToIterate;
	int animationTime;
	bool animationLoop;

	int frameAnimationTime;
	vec2 originalPosIndex;

protected:
	void particularUpdate(UIState* finalStateBasicParameters);

public:
	UIDynamicTextureState();
			
	UIState* clone();

	void setRowsToIterate(int value){ this->rowsToIterate = value; }
	int getRowsToIterate(){ return this->rowsToIterate; }

	void setAnimationTime(int value){ this->animationTime = value; }
	int getAnimationTime(){ return this->animationTime; }

	void setAnimationLoop(bool value){ this->animationLoop = value; }
	bool isAnimationLoop(){ return this->animationLoop; }

	void setPositionIndex(vec2 value);
};