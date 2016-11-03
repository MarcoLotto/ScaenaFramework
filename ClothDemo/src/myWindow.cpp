/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "MeshFactory.h"
#include "VideoConfiguration.h"
#include "GraphicDevice.h"
#include "MemoryManager.h"
#include "FormsManager.h"
#include "PipelineBuilder.h"
#include "Logger.h"
#include "myWindow.h"

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::mat4 mat4;
typedef glm::mat3 mat3;

int fps();
RenderPipeline* renderPipeline;

myWindow::myWindow()
{	
	// Cargo los recursos como el device, ui controller, etc
	this->init();
}

//********************************************************************************************
//									RENDER DE LA ESCENA
//********************************************************************************************
void myWindow::OnRender(){
	if(renderPipeline != NULL){
		renderPipeline->render();
		fps();	
	}
}

int fps(){
	static int frame=0, time=0, timebase=0;
	int fps=0;
	frame++;
	time = WindowConnector::GetOSCurrentTime();

	if (time - timebase > 1000) {
		fps = frame*1000.0/(time-timebase);
	 	timebase = time;
		frame = 0;
		Logger::getInstance()->logInfo(new Log("FPS=" + StringUtils::toString(fps)));
	}
	return fps;
}

//********************************************************************************************
//									UPDATES DE LA ESCENA
//********************************************************************************************
void myWindow::OnUpdate(){	
	// Actualizo la interfaz de usuario
	this->uiController->update();
}

//********************************************************************************************
//									CARGA DEL MOTOR
//********************************************************************************************

void  myWindow::init()
{	
	// Inicializo el logguer
	Logger::getInstance("./generalLog.txt", "./errorLog.txt", true, true, true);

	// Inicializo el device
	GraphicDevice::getInstance()->initiate();

	// Seteo el color de con que se limpia la pantalla
	GraphicDevice::getInstance()->clearColor(0.0f, 0.0f, 0.0f, 0.0f);
	GraphicDevice::getInstance()->enableMode(GraphicDevice::getInstance()->getConstantCatalog()->getDepthTestFlag());
	
	//Inicializo la UI
	this->uiController = new UIController();	
}

void setRenderPipelineConfiguration(UIController* uiController){
	renderPipeline = PipelineBuilder::getRenderPipelineForUIOnly(uiController);
}

void generatePipeline(int width, int height, UIController* uiController){		
	static bool testOneTime = true;
	if(testOneTime){
		testOneTime = false;
		//El menu de creacion de indumentaria
		FormsManager::getInstance(uiController)->initializeMenuForms();
	}
	// Configuro el render pipeline inicial
	setRenderPipelineConfiguration(uiController);
	FormsManager::getInstance()->menuStatusBarForm->setVisible(true);
	FormsManager::getInstance()->shirtsTeamBuilderForm->setVisible(true);
}	


void myWindow::OnResize(int w, int h) 
{
	GraphicDevice::getInstance()->setViewport(0, 0, w, h);
	this->uiController->setScreenSize(vec2(w, h));
	generatePipeline(w, h, this->uiController);
}

void  myWindow::OnClose(void)
{
	//Libero la memoria de los buffers
	MemoryManager::getInstance()->freeAllBuffers();
	MeshFactory::getInstance()->destroy();
}

void myWindow::OnMouseDown(int button, int x, int y)
{	
	//Envio la tecla al this->getUIController(), el deberia encargarse de todo
	this->uiController->manageClick(vec2(x, y), button);
}

void myWindow::OnMouseMove(int x, int y){	
}

void myWindow::OnLeftMouseDrag(int x, int y){	
	this->uiController->setMouseActualCoords(vec2(x, y));
}

void myWindow::OnMouseUp(int button, int x, int y)
{	
	this->uiController->manageClickRelease(vec2(x, y), button);
}

void myWindow::OnMouseWheel(int nWheelNumber, int nDirection, int x, int y)
{
}

void myWindow::OnKeyDown(int nKey, char cAscii)
{       
	//Establezco que las teclas no representables en ascii (como las teclas de direcci{on), tendran un codigo ascii negativo
	if(cAscii == 0)
		cAscii -= nKey;

	//Envio la tecla al this->getUIController(), el deberia encargarse de todo
	this->uiController->manageKeyDown(cAscii);
	
	switch(cAscii){

		case 27: this->Close();
				break;	
		case '1': 
				SetFullscreen(true);
				break;
		case '2': 
				SetFullscreen(false);
			break;	
		default: ;
	}
};

void myWindow::OnKeyUp(int nKey, char cAscii)
{
	//Envio la tecla al this->getUIController(), el deberia encargarse de todo
	this->uiController->manageKeyUp(cAscii);

		switch(cAscii){

		case '1': 
				break;
		case '2': 
				break;	
		default: ;

	}

};

