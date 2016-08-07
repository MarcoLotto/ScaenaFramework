/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "ConnectorImplementation.h"
#include "TouchEvent.h"
#include <string>
using namespace std;

//**************************************************************************************************************
// Interfaz generica para los conectores que vinculan la aplicacion grafica con la ventana del sistema operativo
//**************************************************************************************************************

class WindowConnector : public ConnectorImplementation{

private:
	static string baseApplicationPath, configurationPath;

public:
	//**************************************************************************

	// Llamar antes de crear un objeto de esta clase. Se utiliza para inicializar el contexto (ej. glut)
	static void PreInitialize();

	WindowConnector();
	virtual ~WindowConnector();	

	// Se utiliza para empezar el loop de render. Quizas esto no sea necesario usarlo siempre, pero lo es en el caso de GLUT. De este metodo no se retorna.
	void initiateRenderingLoop();	

	//**************************************************************************

	// **Pueden redefinirse en clases que implementen. Seran llamadas en diversos puntos del programa****************
      
    //! Called when window requires paint
    virtual void OnRender(void){};
      
    //! Called when window requires update
    virtual void OnUpdate(void){};
      
    //! Called when Window is resized
    virtual void OnResize(int nWidth, int nHeight){};
    
    //! Se utiliza para determinar las dimensiones de pantalla en las que se puede hacer click (por ejemplo, en IOS se hace un downscale
    //! porque hay mas densidad de pixels que puntos de toque en la pantalla, por lo que esto es diferente al onResize de la pantalla).
    virtual void OnScreenUIDimensionsResize(int nWidth, int nHeight){};
      
    //! Called when Window is created
    virtual void OnInit(void){};
      
    //! Called when Window is closed.
    virtual void OnClose(void){};
      
    //! Called when Mouse button is pressed
    virtual void OnMouseDown(int button, int x, int y){}; 
      
    //! Called when Mouse button is released
    virtual void OnMouseUp(int button, int x, int y){}; 
      
    //! Called when Mouse is moved (without pressing any button)
    virtual void OnMouseMove(int x, int y){};
      
    //! Called while Left Mouse button is pressed.
    virtual void OnLeftMouseDrag(int x, int y){};
      
    //! Called when mouse wheel is used
    virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y){};
      
    //! Called when a key is pressed on keyboard
    virtual void OnKeyDown(int nKey, char cAscii){};
      
    //! Called when a key is released on keyboard
    virtual void OnKeyUp(int nKey, char cAscii){}; 
	
	//! Es llamado cuando se realiza un evento con una pantalla o panel tactil
	virtual void OnTouchEvent(TouchEvent* event){};

	//! Actualiza la informacion de los sensore
	virtual void OnSensorsUpdate(SensorsData data){};
      
    // ***Comandos****************
      
    //! Force Repaint
    void Repaint();
      
    //! Sets window to fullscreen or windowed mode.
    void  SetFullscreen(bool bFullscreen);
      
    //! Hide the window
    void Hide();
      
    //! Show the window
    void Show();
     
    //! Close the window
    void  Close(); 
	
	//! Pide el tiempo actual en milisegundos al sistema operativo
	static int GetOSCurrentTime();

	//! Solicita la escritura de una cadena por stdout del sistema operativo
	static void writeToStdout(const char* message);
	
	//! Cambia entre el back y el front buffer, y finaliza con todas las operaciones pendientes en GPU
	static void SwapBuffers();

	//! Devuelve la ruta del directorio base de la aplicacion
    static string getBaseApplicationPath();

	//! Setea la ruta del directorio base de la aplicacion
	static void setBaseApplicationPath(string basePath);
    
    //! Devuelve la ruta del directorio de archivos de configuracion de la aplicacion
    static string getApplicationConfigurationPath();
    
    //! Setea la ruta del directorio de archivos de configuracion de la aplicacion
    static void setApplicationConfigurationPath(string basePath);

	//! Abre/cierra el teclado (usualmente por soft) del dispositivo 
	static void openKeyboard();
	static void closeKeyboard();

	//! Hace vibrar (si esta disponible) al dispositivo
	static void vibrate(int timeInMilliseconds);
};
