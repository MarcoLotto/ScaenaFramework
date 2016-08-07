/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Log.h"
#include "StringUtils.h"
#include <list>
using namespace std;

class Logger{

private:
	static Logger* instance;
	list<Log*>* logs;
	string generalLogFilePath;
	string errorLogFilePath;

	// Indica si se deben mostrar los logs en stdout al producirse los mismos
	bool showInfoLogsOnStdout;
	bool showDebugLogsOnStdout;
	bool showErrorLogsOnStdout;

	Logger();
	virtual ~Logger();
	void writeToErrorFile(string lineToWrite);
	void writeToFile(string lineToWrite, ofstream* file);

public:
	static Logger* getInstance(string generalLogFile, string errorLogFile, bool showInfoLogsOnStdout, bool showDebugLogsOnStdout, bool showErrorLogsOnStdout);
	static Logger* getInstance();

	void logDebug(Log* log);
	void logInfo(Log* log);
	void logError(Log* log);

	void showAllLogsOnScreen();
	void writeAllLogsInFile(string filename);

	void setGeneralLogFilePath(string filename){ this->generalLogFilePath = filename; }
	void setErrorLogFilePath(string filename){ this->errorLogFilePath = filename; }

	void setShowInfoLogsOnStdout(bool showValue){ this->showInfoLogsOnStdout = showValue; }
	void setShowDebugLogsOnStdout(bool showValue){ this->showDebugLogsOnStdout = showValue; }
	void setShowErrorLogsOnStdout(bool showValue){ this->showErrorLogsOnStdout = showValue; }

	void cleanLogs();
};