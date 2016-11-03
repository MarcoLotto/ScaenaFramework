/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <string>
using namespace std;

class StringUtils{

private:

public:
	static string toString(char value);
	static string toString(float value);
	static string toString(int value);
	static string toString(unsigned int value);

	// Busca un carater en una cadena a partir de una posicion, devuelve la posicion del caracter en la cadena (relativa a startPosition) o -1 si no esta.
	static int find(const char* data, int startPosition, int dataSize, char characterToLookFor);

	// Busca un texto en una cadena a partir de una posicion, devuelve la posicion del caracter en la cadena (relativa a startPosition) o -1 si no esta.
	static int find(const char* data, int startPosition, int dataSize, char* textToLookFor);
	static int find(string data, string textToLookFor);

	// Indica se una cadena esta contenida en otra cadena
	static bool contains(string source, string textToLookFor);

	// Guarda en destination un substring de source
	static void substring(char* destination, const char* source, int startPosition, int length);
	static string substring(string source, int startPosition, int length);

	// Reemplaza todos los caracteres que cumplan con originalChar por el caracter definido en finalChar
	static string replaceAll(string source, char originalChar, char finalChar);

	// Reemplaza todos los caracteres que cumplan con originalChar por los caracteres definidos en finalSrc
	static string replaceAll(string source, char originalChar, string finalSrc);

	// Encodea un parametro de url para poder enviarse via http
	static string encodeUrlParameter(string plainParameter);
};