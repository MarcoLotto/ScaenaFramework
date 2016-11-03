#pragma once
#include <string>
#include "MockGraphicDevice.h"
using namespace std;

class Test {

protected:
	void assertEquals(string v1, string v2);
	void assertEquals(int v1, int v2);
	void assertEquals(float v1, float v2);
	
	void assertGraterThan(int v1, int v2);
	void assertGraterThan(float v1, float v2);

	void assertSmallerThan(int v1, int v2);
	void assertSmallerThan(float v1, float v2);

	void assertTrue(string field, bool v);
	void assertDeviceAccess(MockGraphicDevice* device, string deviceMethodName, int v2);

public:
	// Debe colocarse aqui todo el codigo que se correra
	virtual void when() = 0;

	// Debe colocarse aqui las verificaciones de lo corrido en el "when". Recibe el graphic device de mock para consultas
	virtual void then(MockGraphicDevice* device) = 0;

	// Determina el nombre del test
	virtual string getName() = 0;
};