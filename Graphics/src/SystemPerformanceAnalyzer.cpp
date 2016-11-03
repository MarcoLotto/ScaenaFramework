/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SystemPerformanceAnalyzer.h"
#include "VideoConfiguration.h"

SystemPerformanceAnalyzer* SystemPerformanceAnalyzer::instance = NULL;

SystemPerformanceAnalyzer* SystemPerformanceAnalyzer::getInstance(){
	if(instance == NULL){
		instance = new SystemPerformanceAnalyzer();
	}
	return instance;
}

// Se puede indicar (opcionalmente) el inicio de una etapa de profile
void SystemPerformanceAnalyzer::indicateProfilingStart(string profileName){
	if(VideoConfiguration::getInstance()->isDebugMode())
		printf("--Frame profile------------------\n");
}

// Se puede indicar (opcionalmente) el fin de una etapa de profile
void SystemPerformanceAnalyzer::indicateProfilingEnd(){
	if(VideoConfiguration::getInstance()->isDebugMode())
		printf("---------------------------------\n");
}

// Imprime una estadistica particular (no promedios sino actuales) en standart output
void SystemPerformanceAnalyzer::printProfileOnScreen(string name, float timeInMilliseconds){		
	printf("%s, %fms\n", name.c_str(), timeInMilliseconds);	
}

// Actualiza la informacion de un profile (saca el promedio con las anteriores) y la crea si no existe
void SystemPerformanceAnalyzer::updateProfileData(string name, float timeInMilliseconds){
	map<string, float>::iterator it = this->averangeTimes->find(name);
	if(it != this->averangeTimes->end()){
		// No es promedio real porque tiene mucho peso la ultima medición, pero para los efectos va
		it->second = (it->second + timeInMilliseconds) / 2.0f;
	}
	else{
		this->averangeTimes->insert(std::pair<string, float>(name, timeInMilliseconds));
	}
	// Si estoy en modo debug, el profiling va a la consola
	if(VideoConfiguration::getInstance()->isDebugMode())
		this->printProfileOnScreen(name, timeInMilliseconds);
}
