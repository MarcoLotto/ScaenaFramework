/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "FrameBuffer.h"

// La finalidad de este frame buffer especial es para renderizar directamente en pantalla.
class ToScreenFrameBuffer :  public FrameBuffer{

private:
	ApiTexture* mockTexture;
	void createToScreenFrameBuffer();

public:
	ToScreenFrameBuffer();
	virtual ~ToScreenFrameBuffer();

	virtual void bind();
	virtual void bindAndLockOutputs();
};