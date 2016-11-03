/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "WindowConnector.h"
#include "MainNativeActivity.h"
#include "NativeActivityConnector.h"
#include <time.h>
#include <android/log.h>

#define SHOW_KEYBOARD_JAVA_METHOD_NAME "showSoftKeyboard"
#define HIDE_KEYBOARD_JAVA_METHOD_NAME "hideSoftKeyboard"
#define VIBRATE_JAVA_METHOD_NAME "vibrate"

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
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (now.tv_sec*1000) + now.tv_nsec/1000000;
}

//! Solicita la escritura de una cadena por stdout del sistema operativo
void WindowConnector::writeToStdout(const char* message){
	__android_log_print(ANDROID_LOG_ERROR, "SCAENA", message);
}

//! Cambia entre el back y el front buffer, y finaliza con todas las operaciones pendientes en GPU
void WindowConnector::SwapBuffers(){
	swapBuffers();  // Definida en MainNativeActivity
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
	// Llamamos a un metodo implementado en el lado java para abrir el teclado
	callVoidJavaMethod(SHOW_KEYBOARD_JAVA_METHOD_NAME);
}
void WindowConnector::closeKeyboard(){
	callVoidJavaMethod(HIDE_KEYBOARD_JAVA_METHOD_NAME);
}

//! Hace vibrar (si esta disponible) al dispositivo
void WindowConnector::vibrate(int timeInMilliseconds){
	callVoidJavaMethod(VIBRATE_JAVA_METHOD_NAME, timeInMilliseconds);
}



