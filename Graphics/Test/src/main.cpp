#include "Logger.h"
#include "TestManager.h"
#include "MockGraphicDevice.h"
#include <iostream>

void main(){
	// Iniciamos el logger
	Logger::getInstance("TestGeneralLogs.txt", "TestErrorLogs.txt", true, true, true);

	// Invocamos al test manager para que corra las configuracion
	TestManager* testManager = new TestManager();
	testManager->runTests();

	std::string exitVar;
	std::cin >> exitVar;
}

