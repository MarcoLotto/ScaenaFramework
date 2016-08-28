//=============================================================================
// Sample Application for GLEW, and cwc Application/Window class using freeglut
//=============================================================================

#include "GraphicDevice.h"
#include <myWindow.h>
#include <iostream>


//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

int main(void)
{
	//Digo que voy a armar una aplicacion OpenGL
	GraphicDevice::bindApi(OPENGL_API);
	
	myWindow::PreInitialize();
	myWindow* myWin = new myWindow();
	myWin->initiateRenderingLoop();
	
	delete myWin;	
	
	return 0;
}

//-----------------------------------------------------------------------------

