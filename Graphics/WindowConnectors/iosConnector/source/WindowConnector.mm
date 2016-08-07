/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "WindowConnector.h"
#include <asl.h>
#include <sys/time.h>
#include "GameViewController.h"

// Defino la ruta base de archivos de la aplicacion por defecto
string WindowConnector::baseApplicationPath = "./";
string WindowConnector::configurationPath = "./";

void WindowConnector::PreInitialize(){

}

WindowConnector::WindowConnector(){	
}

WindowConnector::~WindowConnector(){
	
}

// Se utiliza para empezar el loop de render. Quizas esto no sea necesario usarlo siempre, pero lo es en el caso de GLUT. De este metodo no se retorna.
void WindowConnector::initiateRenderingLoop(){
	
}


//-----------------------------------------------------------------------------

void WindowConnector::Repaint()
{
}

//-----------------------------------------------------------------------------

void  WindowConnector::SetFullscreen(bool bFullscreen)
{
}

//-----------------------------------------------------------------------------
   
void WindowConnector::Hide()
{
  
}

//-----------------------------------------------------------------------------
   
void WindowConnector::Show()
{
  
}   

//-----------------------------------------------------------------------------   
   
void  WindowConnector::Close()
{
}

//-----------------------------------------------------------------------------
//! Pide el tiempo actual en milisegundos al sistema operativo
int WindowConnector::GetOSCurrentTime(){
	timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

//! Solicita la escritura de una cadena por stdout del sistema operativo
void WindowConnector::writeToStdout(const char* message){
    fprintf(stderr, "%s", message);
}

//! Cambia entre el back y el front buffer, y finaliza con todas las operaciones pendientes en GPU
void WindowConnector::SwapBuffers(){
    appSwapBuffers();
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
	// TODO
}
void WindowConnector::closeKeyboard(){
	// TODO
}

//! Hace vibrar (si esta disponible) al dispositivo
void WindowConnector::vibrate(int timeInMilliseconds){
	// TODO
}



