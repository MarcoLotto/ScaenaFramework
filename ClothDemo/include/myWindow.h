/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

#include "WindowConnector.h"
#include "UIController.h"

class myWindow : public WindowConnector
{
private:
	UIController* uiController;

public:
	myWindow();

	void OnUpdate(void);
	void OnRender(void);	
	void init();

	// When OnInit is called, a render context is already available!
	virtual void OnInit(){};

	virtual void OnResize(int w, int h);
	virtual void OnClose(void);
	virtual void OnMouseDown(int button, int x, int y);
	virtual void OnMouseUp(int button, int x, int y);
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y);
	virtual void OnMouseMove(int x, int y);
	virtual void OnLeftMouseDrag(int x, int y);
	virtual void OnKeyDown(int nKey, char cAscii);
	virtual void OnKeyUp(int nKey, char cAscii);
};
