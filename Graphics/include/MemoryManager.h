/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

#include <vector>
using namespace std;

#define CLAVE_MAX 28000

typedef struct EntradaPF{
	int clave;
	float* buffer;
	int tamanio;
};
typedef struct EntradaINT{
	int clave;
	unsigned int* buffer;
	int tamanio;
};

/***************************************************************************************
************************************MemoryManager***************************************
*Verifica si un buffer ya existe en memoria, de ser asi no sigue almacenando memoria,
*sino que devuelve el buffer ya existente.
****************************************************************************************/
class MemoryManager{

private:
	vector<EntradaPF*> bufferListPF;
	vector<EntradaINT*> bufferListINT;
	static MemoryManager* instance;

	//Aplica una funcion de hash al buffer para conseguir su clave
	int hashFunction(float* buffer, int tam);
	int hashFunction(unsigned int* buffer, int tam);
	//Compara todo el contenido de un buffer con el otro
	bool compararBuffer(EntradaPF* entrada1, EntradaPF* entrada2);
	bool compararBuffer(EntradaINT* entrada1, EntradaINT* entrada2);
	
public:
	static MemoryManager* getInstance();

	//Se la pasa un buffer y su tamano. Se verifica si ese buffer ya existe en memoria.
	//De sera asi, se libera el buffer pasado, y se devuelve el bufer en memoria.
	//Si no esta en memoria, se guarda en la lista de buffers el buffer pasado y se devuelve el mismo.
	float* checkAndGetBuffer(float* buffer, int *tam);
	unsigned int* checkAndGetBuffer(unsigned int* buffer, int *tam);

	//Libera toda la memoria de los buffers
	void freeAllBuffers();


};