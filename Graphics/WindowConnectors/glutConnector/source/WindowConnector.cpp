/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "WindowConnector.h"
#include "glApplication.h"
#include "GL/freeglut.h"

// Defino la ruta base de archivos de la aplicacion por defecto
string WindowConnector::baseApplicationPath = "./";
string WindowConnector::configurationPath = "./";
glApplication* app = NULL;

void WindowConnector::PreInitialize(){
	if(app == NULL) {
		app = new glApplication();		
	}
}

WindowConnector::WindowConnector(){	
	// No puedo inicializar acá glApplication porque el constructor padre usa glut
}

WindowConnector::~WindowConnector(){
	if(app != NULL){
		delete app;
		app = NULL;
	}
}

// Se utiliza para empezar el loop de render. Quizas esto no sea necesario usarlo siempre, pero lo es en el caso de GLUT. De este metodo no se retorna.
void WindowConnector::initiateRenderingLoop(){
	if(app != NULL){
		app->run();
	}
	else{
		// Error, falta llamar a WindowConnector::PreInitialize() para inicializar glut
	}
}


//-----------------------------------------------------------------------------

void WindowConnector::Repaint()
{
   glutPostWindowRedisplay(_gWindow);
}

//-----------------------------------------------------------------------------

void  WindowConnector::SetFullscreen(bool bFullscreen)
{
   if (bFullscreen)
   {
      glutFullScreen();
   }
   else
   {
      int w = 640; //glutGet(GLUT_WINDOW_WIDTH);
      int h = 480; //glutGet(GLUT_WINDOW_HEIGHT);
      int x = 0; //glutGet(GLUT_WINDOW_X);
      int y = 0; //glutGet(GLUT_WINDOW_Y);
      
      glutPositionWindow(x,y);
	  glutReshapeWindow(w,h);
   }
}

//-----------------------------------------------------------------------------
   
void WindowConnector::Hide()
{
   glutHideWindow();
}

//-----------------------------------------------------------------------------
   
void WindowConnector::Show()
{
   glutShowWindow();
}   

//-----------------------------------------------------------------------------   
   
void  WindowConnector::Close()
{
   glutDestroyWindow(_gWindow);
}

//-----------------------------------------------------------------------------
//! Pide el tiempo actual en milisegundos al sistema operativo
int WindowConnector::GetOSCurrentTime(){
	return glutGet(GLUT_ELAPSED_TIME);
}

//! Solicita la escritura de una cadena por stdout del sistema operativo
void WindowConnector::writeToStdout(const char* message){
	printf("%s", message);
}

//! Cambia entre el back y el front buffer, y finaliza con todas las operaciones pendientes en GPU
void WindowConnector::SwapBuffers(){
	glutSwapBuffers();
}

//! Devuelve la ruta del directorio base de la aplicacion
string WindowConnector::getBaseApplicationPath(){
	return baseApplicationPath;
}

//! Setea la ruta del directorio base de la aplicacion
void WindowConnector::setBaseApplicationPath(string basePath){	
	baseApplicationPath = basePath; 
}

//! Devuelve la ruta del directorio base de la aplicacion
string WindowConnector::getApplicationConfigurationPath(){
    return configurationPath;
}

//! Setea la ruta del directorio base de la aplicacion
void WindowConnector::setApplicationConfigurationPath(string basePath){
    configurationPath = basePath;
}

//! Abre/cierra el teclado (usualmente por soft) del dispositivo 
void WindowConnector::openKeyboard(){
	// En PC no es necesario implementar este metodo (ya hay teclado)
}
void WindowConnector::closeKeyboard(){
	// En PC no es necesario implementar este metodo
}

//! Hace vibrar (si esta disponible) al dispositivo
void WindowConnector::vibrate(int timeInMilliseconds){
	// La PC no vibra por ahora
}

