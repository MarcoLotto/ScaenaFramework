/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "GpuTimeProfiler.h"
#include "GraphicDevice.h"

// Genera un nuevo profile y devuelve su identificador (genera memoria no usar todo el tiempo)
unsigned int GpuTimeProfiler::reservProfile(){
	return GraphicDevice::getInstance()->generateQuery();
}

// Dado un handler, empieza el profile de la GPU. Se medira cuando tiempo pasa entre esta llamada y la llamada a endProfile (tiempo GPU)
void GpuTimeProfiler::startProfile(unsigned int profileHandler){
	GraphicDevice::getInstance()->beginQuery(profileHandler, GraphicDevice::getInstance()->getConstantCatalog()->getTimeElapsedQueryType());
}

// Marca en donde termina el profile de la GPU
void GpuTimeProfiler::endProfile(){
	GraphicDevice::getInstance()->endQuery(GraphicDevice::getInstance()->getConstantCatalog()->getTimeElapsedQueryType());
}

// Solicita el tiempo de GPU registrado para el handler. Tener cuidado porque si no esta listo el resultado se produce un stall de CPU hasta que este listo.
unsigned int GpuTimeProfiler::requestProfileResult(unsigned int profileHandler){
	return GraphicDevice::getInstance()->getQueryResultAsUInt(profileHandler);
}

// Borra de la memoria el profiler
void GpuTimeProfiler::deleteProfile(unsigned int profileHandler){
	GraphicDevice::getInstance()->deleteQuery(profileHandler);
}