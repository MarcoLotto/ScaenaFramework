/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <string>
#include <map>
#include <list>
using namespace std;

class SystemPerformanceAnalyzer{

private:	
	map<string, float>* averangeTimes;

	static SystemPerformanceAnalyzer* instance;
	SystemPerformanceAnalyzer(){ this->averangeTimes = new map<string, float>(); }
	virtual ~SystemPerformanceAnalyzer(){ delete this->averangeTimes; }

	// Imprime una estadistica particular (no promedios sino actuales) en standart output
	void printProfileOnScreen(string name, float timeInMilliseconds);

public:
	static SystemPerformanceAnalyzer* getInstance();

	// Se puede indicar (opcionalmente) el inicio de una etapa de profile
	void indicateProfilingStart(string profileName);

	// Actualiza la informacion de un profile (saca el promedio con las anteriores) y la crea si no existe
	void updateProfileData(string name, float timeInMilliseconds);

	// Se puede indicar (opcionalmente) el fin de una etapa de profile
	void indicateProfilingEnd();

	// Toma un snapshot del estado (tiempos promedios) del sistema. 
	void takeSnapShot(string profileFilename){ /* TODO */ }

	// Recupera los tiempos promedios guardados anteriormente. Devuelve una lista con esos tiempos
    map<string, float>* recoverSnapShot(string profileFilename){ /* TODO */ return NULL; }

	// Devuelve los tiempos promedios del sistema
	map<string, float>* getAverangeTimes(){ return this->averangeTimes; }

	// Evalua posibles problemas de performance en el sistema
    list<string>* evaluatePerformanceProblems(){ /* TODO */ return NULL; }
};