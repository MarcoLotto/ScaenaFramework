#pragma once

#include <list>
#include "Test.h"

using namespace std;

class TestConfiguration {

private:
	list<Test*> tests;

	void configureTests();

public:
	TestConfiguration(){
		this->configureTests();
	}

	list<Test*> getTests(){ return this->tests; };
};
