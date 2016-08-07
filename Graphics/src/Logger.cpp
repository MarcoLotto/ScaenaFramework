/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "Logger.h"
#include "LoggerNotInitializedException.h"
#include "WindowConnector.h"
#include <iostream>
#include <fstream>
using namespace std;

Logger* Logger::instance = NULL;

Logger::Logger(){
	this->logs = new list<Log*>();
}
Logger::~Logger(){
	delete this->logs;
}

Logger* Logger::getInstance(){
	if(instance == NULL){
		throw new LoggerNotInitializedException();
	}
	return instance;
}

Logger* Logger::getInstance(string generalLogFile, string errorLogFile, bool showInfoLogsOnStdout, bool showDebugLogsOnStdout, bool showErrorLogsOnStdout){
	if(instance == NULL){
		instance = new Logger();
		instance->setErrorLogFilePath(errorLogFile);
		instance->setGeneralLogFilePath(generalLogFile);
		instance->setShowInfoLogsOnStdout(showInfoLogsOnStdout);
		instance->setShowDebugLogsOnStdout(showDebugLogsOnStdout);
		instance->setShowErrorLogsOnStdout(showErrorLogsOnStdout);
	}
	return instance;
}

void Logger::logDebug(Log* log){
	log->setType(DEBUG_LOG_TYPE);
	this->logs->push_back(log);

	// Si se se indico escribo a stdout
	if(this->showDebugLogsOnStdout){
		WindowConnector::writeToStdout(("Debug Log: " + log->getMessage() + "\n").c_str());
	}
}
void Logger::logInfo(Log* log){
	log->setType(INFO_LOG_TYPE);
	this->logs->push_back(log);

	// Si se se indico escribo a stdout
	if(this->showInfoLogsOnStdout){
		WindowConnector::writeToStdout(("Info Log: " + log->getMessage() + "\n").c_str());
	}
}
void Logger::logError(Log* log){
	log->setType(ERROR_LOG_TYPE);
	this->logs->push_back(log);

	// Si se se indico escribo a stdout
	if(this->showErrorLogsOnStdout){
		WindowConnector::writeToStdout(("Error Log: " + log->getMessage() + "\n").c_str());
	}

	//Como es un log de error, lo escribo directo al log
	this->writeToErrorFile("Error Log: " + log->getMessage() + "\n");
}

void Logger::showAllLogsOnScreen(){
	int logNumber = 1;
	std::list<Log*>::iterator it = this->logs->begin();
	while(it != this->logs->end()){
		Log* log = (*it);
		if(log->getType() == INFO_LOG_TYPE){
			printf("%d.Info Log: %s\n", logNumber, log->getMessage().c_str());
		}
		else if(log->getType() == DEBUG_LOG_TYPE){
			printf("%d.Debug Log: %s\n", logNumber, log->getMessage().c_str());
		}
		else if(log->getType() == ERROR_LOG_TYPE){
			printf("%d.Error Log: %s\n", logNumber, log->getMessage().c_str());
		}
		logNumber++;
		++it;
	}
}
void Logger::writeAllLogsInFile(string filename){
	 ofstream file;
	 file.open (this->generalLogFilePath.c_str(), ios::out);
	
	int logNumber = 1;
	std::list<Log*>::iterator it = this->logs->begin();
	while(it != this->logs->end()){
		Log* log = (*it);
		if(log->getType() == INFO_LOG_TYPE){
			this->writeToFile(logNumber + ".Info Log: " + log->getMessage(), &file);
		}
		else if(log->getType() == DEBUG_LOG_TYPE){
			this->writeToFile(logNumber + ".Debug Log: " + log->getMessage(), &file);
		}
		else if(log->getType() == ERROR_LOG_TYPE){
			this->writeToFile(logNumber + ".Error Log: " + log->getMessage(), &file);
		}
		logNumber++;
		++it;
	}
	file.close();
}

void Logger::writeToFile(string lineToWrite, ofstream* file){
   *file << lineToWrite << "\n";
 }

void Logger::writeToErrorFile(string lineToWrite){
  ofstream myfile;
  myfile.open (this->errorLogFilePath, ios::app);
  myfile << lineToWrite << "\n";
  myfile.close();
}

void Logger::cleanLogs(){
	std::list<Log*>::iterator it = this->logs->begin();
	while(it != this->logs->end()){
		delete (*it);
		it = this->logs->erase(it);
	}
}
