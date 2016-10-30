/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "GraphicDevice.h"
#include <stdlib.h>
#include "ApiNotBindException.h"
#include "ApiVariableTypeDoesNotExistException.h"

#include "OpenglDevice.h"
#include "OpenglES3Device.h"
#include "OpenglES2Device.h"

GraphicDevice* GraphicDevice::instance = NULL;
unsigned int GraphicDevice::defaultFrameBuffer = 0;
int GraphicDevice::bindedApi = -1;

GraphicDevice* GraphicDevice::getInstance(){
	if(instance == NULL){
		//Lanzo excepcion, deberia haberse bindeado un api
		throw new ApiNotBindException();
	}
	return instance;
}

void GraphicDevice::bindApi(int apiId){
    if(instance != NULL){
		instance->destroy();
		delete instance;		
	}
	//Inicializo el API pedido
	// Se deben definir en el preprocesador las opciones disponibles para la plataforma en donde se compila
	switch(apiId){
		#ifdef OPENGL4
		case OPENGL_API:
			instance = new OpenglDevice(); 
		break;
		#endif
		#ifdef OPENGLES3
		case OPENGLES_3_API:
			instance = new OpenglES3Device(); 
		break;
		#endif
		#ifdef OPENGLES2
		case OPENGLES_2_API:
			instance = new OpenglES2Device(); 
		break;
		#endif	 
		default:
			throw new ApiVariableTypeDoesNotExistException("El API informada al GraphicDevice no es correcta (falta definir parametro del preprocesador?)");
		break;
	}
	// Le seteo al GLSLProgram que implementacion de API voy a usar
	GLSLProgram::bindApi(apiId);
    bindedApi = apiId;
}

void GraphicDevice::bindCustomApi(GraphicDevice* graphicDevice){
    if(instance != NULL){
		instance->destroy();
		delete instance;		
	}
	//Inicializo el API pedido
	instance = graphicDevice;

	// Le seteo al GLSLProgram que implementacion de API voy a usar
	GLSLProgram::bindApi(MOCK_API);
}

void GraphicDevice::setDefaultFrameBuffer(unsigned int frameBufferHandler){
    defaultFrameBuffer = frameBufferHandler;
}

