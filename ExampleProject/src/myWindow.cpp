/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "CamaraPrimeraPersona.h"
#include "ShaderManager.h"
#include "GenericUtils.h"
#include "MeshFactory.h"
#include "SceneLoader.h"
#include "VideoConfiguration.h"
#include "PipelineSelectorForm.h"

#include "GraphicDevice.h"
#include "MemoryManager.h"

#include "TransformableMesh.h"
#include "BasicForceMeshDeformationTFShader.h"
#include "MeshKeyframeAnimationTF.h"

#include "ExamplePipelineBuilder.h"
#include "Logger.h"
#include <myWindow.h>

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::mat4 mat4;
typedef glm::mat3 mat3;

int fps();

bool arriba=false, abajo=false, izq=false, der=false;
bool adelante=false, atras=false;

Scene* scene;
vec2 actualMouseCoords;
CamaraPrimeraPersona camara;
RenderPipeline* renderPipeline;
PipelineSelectorForm* pipelineSelectorForm;
PipelineSelectionIndex selectedPipelineIndex;

myWindow::myWindow()
{	
	// Cargo los recursos como el device, ui controller, etc
	this->init();
}

//********************************************************************************************
//									RENDER DE LA ESCENA
//********************************************************************************************
void myWindow::OnRender(){
	ShaderManager::getInstance()->update(camara.getEyePosition());
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


void keyboardAction(){	
	float elapsedTime = (float)WindowConnector::GetOSCurrentTime();
	static float currentTime = elapsedTime;
	currentTime = (elapsedTime - currentTime) / 50.0f;
	
	if(arriba)	
		camara.moveUp(currentTime);		
	if(abajo)
		camara.moveUp(-currentTime);		
	if(izq)
		camara.moveSide(currentTime);		
	if(der)
		camara.moveSide(-currentTime);		
	if(adelante)
		camara.moveFront(currentTime);		
	if(atras)
		camara.moveFront(-currentTime);		

	currentTime = elapsedTime;
}

vec2 coordMouse = vec2(0.0f);
void mouseMove(int x, int y){	
	actualMouseCoords = vec2(x, y);  // REVIEW

	static vec2 angulo = vec2(0.0f);
	float sensibilidad = 0.6f;

	float diff = (x - coordMouse.x)* sensibilidad;
	if(GenericUtils::modulo(diff) >= 0.0f){		
		angulo.x = camara.getEyeRotationYaw() + diff;
		coordMouse.x = x;
		camara.setRotationYaw(angulo.x);
	}
	diff = (y - coordMouse.y)* sensibilidad;
	if(GenericUtils::modulo(diff) >= 0.0f){		
		angulo.y = camara.getEyeRotationPitch() + diff;
		coordMouse.y = y;
		camara.setRotationPitch(angulo.y);
	}	
}

//********************************************************************************************
//									UPDATES DE LA ESCENA
//********************************************************************************************
void myWindow::OnUpdate(){	
	// REVIEW: Esto no seria necesario en este ejemplo
	keyboardAction();	
	this->uiController->setMouseActualCoords(actualMouseCoords);  
	
	//Actualizo la escena
	if(scene != NULL)
		scene->update();

	// Actualizo la camara
	camara.update();

	// Actualizo la interfaz de usuario
	this->uiController->update();

	// REVIEW: Hacemos girar a la camara
	camara.setRotationYaw(TimeManager::getOSTimeInMilliseconds() / 100.0f);  // BORRAR
	camara.setRotationPitch(-90 + 10.0f * glm::cos(TimeManager::getOSTimeInMilliseconds() / 4000.0f));  // BORRAR

	// Vamos a checkear el estado del form selector de pipeline para cambiar el pipeline
	this->checkPipelineSelected();
}

void configurarCamaraTerceraPersonaParaTest(){
	camara.setRotationPitch(-90.0f);
	camara.setRotationYaw(180.0f);
	camara.setEyePosition(vec3(0.0f, 0.0f, 1.0f));
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
	
	//Configuro la camara
	configurarCamaraTerceraPersonaParaTest();
	
	//Inicializo la UI
	this->uiController = new UIController();	
}

void setRenderPipelineConfiguration1(UIController* uiController){
	scene->setCurrentCamera(&camara);
	renderPipeline = ExamplePipelineBuilder::getRenderPipelineConfigForward(uiController, scene);
}

void setRenderPipelineConfiguration2(UIController* uiController){
	scene->setCurrentCamera(&camara);
	renderPipeline = ExamplePipelineBuilder::getRenderPipelineConfigForwardAndBloom(uiController, scene);
}

void setRenderPipelineConfiguration3(UIController* uiController){
	scene->setCurrentCamera(&camara);
	renderPipeline = ExamplePipelineBuilder::getRenderPipelineConfigDeferred(uiController, scene);
}

void setRenderPipelineConfiguration4(UIController* uiController, bool nearDepthOfField){
	scene->setCurrentCamera(&camara);
	renderPipeline = ExamplePipelineBuilder::getRenderPipelineConfigDeferredAndDepthOfField(uiController, scene, nearDepthOfField);
}

void setRenderPipelineConfiguration5(UIController* uiController){
	scene->setCurrentCamera(&camara);
	renderPipeline = ExamplePipelineBuilder::getRenderPipelineConfigForwardAndBlur(uiController, scene);
}

void setRenderPipelineConfiguration6(UIController* uiController){
	scene->setCurrentCamera(&camara);
	renderPipeline = ExamplePipelineBuilder::getRenderPipelineConfigForwardAndUI(uiController, scene);
	selectedPipelineIndex = pipelineSelectorForm->getSelectedPipeline();
}

// Esto podria implementarse mas optimo y prolijo con un callback, pero para mantener la simplicidad del ejemplo lo hacemos asi
void myWindow::checkPipelineSelected(){
	if(selectedPipelineIndex != pipelineSelectorForm->getSelectedPipeline()){
		switch(pipelineSelectorForm->getSelectedPipeline()){
			case PipelineSelectionIndex::p1:
				setRenderPipelineConfiguration1(this->uiController);
			break;
			case PipelineSelectionIndex::p2:
				setRenderPipelineConfiguration2(this->uiController);
			break;
			case PipelineSelectionIndex::p3:
				setRenderPipelineConfiguration5(this->uiController);
			break;
			case PipelineSelectionIndex::p4:
				setRenderPipelineConfiguration3(this->uiController);
			break;
			case PipelineSelectionIndex::p5:
				setRenderPipelineConfiguration4(this->uiController, false);
			break;
			case PipelineSelectionIndex::p6:
				setRenderPipelineConfiguration4(this->uiController, true);
			break;
		}
		selectedPipelineIndex = pipelineSelectorForm->getSelectedPipeline();
	}
}

void generatePipeline(int width, int height, UIController* uiController){		
	static bool testOneTime = true;
	if(testOneTime){
		testOneTime = false;
		//El menu de seleccion de pipeline
		pipelineSelectorForm = new PipelineSelectorForm(uiController);
		uiController->addForm(pipelineSelectorForm);

		// La escena
		scene = SceneLoader::loadFromXmlFile("Definitions/Scenes/scene6.xml");
	}
	// Configuro el render pipeline inicial
	setRenderPipelineConfiguration6(uiController);
}	


void myWindow::OnResize(int w, int h) 
{
	GraphicDevice::getInstance()->setViewport(0, 0, w, h);
	this->uiController->setScreenSize(vec2(w, h));
	generatePipeline(w, h, this->uiController);

	// Recalculamos la perspectiva de la camara (para una relacion 16:9 -> 1.77)
	camara.setPerspective(45.0f, (float(w)/float(h)), 0.1f, 1000.0f);
}

void  myWindow::OnClose(void)
{
	//Libero la memoria de los buffers
	MemoryManager::getInstance()->freeAllBuffers();
	MeshFactory::getInstance()->destroy();
}

//***Manejo de camara con el mouse******************************
void myWindow::OnMouseDown(int button, int x, int y)
{	
	//Envio la tecla al this->getUIController(), el deberia encargarse de todo (dejo el resto por ahora)
	this->uiController->manageClick(vec2(x, y), button);

	coordMouse.x = x;
	coordMouse.y = y;

	// Llamo a la funcion que se encarga de informar que se movio el mouse a la camara
	mouseMove(x, y);
}

void myWindow::OnMouseMove(int x, int y){	
	mouseMove(x, y);
}

void myWindow::OnLeftMouseDrag(int x, int y){	
	this->uiController->setMouseActualCoords(vec2(x, y));
}

void myWindow::OnMouseUp(int button, int x, int y)
{	
	//Envio la tecla al this->getUIController(), el deberia encargarse de todo
	this->uiController->manageClickRelease(vec2(x, y), button);

	// REVIEW: Por ahora al soltar el click traemos al menu de seleccion de pipeline
	pipelineSelectorForm->setVisible(true);
	setRenderPipelineConfiguration6(this->uiController);
}

void myWindow::OnMouseWheel(int nWheelNumber, int nDirection, int x, int y)
{
	//camara.setDistanceToPoint(camara.getDistanceToPoint() + nDirection);
}
//**************************************************************


void myWindow::OnKeyDown(int nKey, char cAscii)
{       
	//Establezco que las teclas no representables en ascii (como las teclas de direcci{on), tendran un codigo ascii negativo
	if(cAscii == 0)
		cAscii -= nKey;

	//Envio la tecla al this->getUIController(), el deberia encargarse de todo (dejo el resto por ahora)
	this->uiController->manageKeyDown(cAscii);
	
	switch(cAscii){

		case 27: this->Close();
				break;	
		case 'a': izq = true;
				break;
		case 'd': der = true;
				break;
		case 'w': adelante = true;
				break;
		case 's': atras = true;
				break;
		case 'z': arriba = true;
				break;
		case 'x': abajo = true;
				break;
		case 'q': 
				SetFullscreen(true);
				break;
		case 'e': 
				SetFullscreen(false);
			break;	
		default: ;
	}
};

void myWindow::OnKeyUp(int nKey, char cAscii)
{
	//Envio la tecla al this->getUIController(), el deberia encargarse de todo (dejo el resto por ahora)
	this->uiController->manageKeyUp(cAscii);

		switch(cAscii){

		case 'q': 
				break;
		case 'e': 
				break;	
		//**********************************
		case 'a': izq = false;
				break;
		case 'd': der = false;
				break;
		case 'w': adelante = false;
				break;
		case 's': atras = false;
				break;
		case 'z': arriba = false;
				break;
		case 'x': abajo = false;
				break;
		//***********************************
		default: ;

	}

};

