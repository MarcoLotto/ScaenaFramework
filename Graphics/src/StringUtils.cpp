/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "StringUtils.h"
#include <algorithm>
#include <string>
#include <string.h>
#include "sstream"
using std::string;
using std::stringstream;
using std::ostringstream;

string StringUtils::toString(char value){
	ostringstream ss;
    ss << value;
    return ss.str();
}

string StringUtils::toString(float value){
	ostringstream ss;
    ss << value;
    return ss.str();
}

string StringUtils::toString(int value){
	ostringstream ss;
    ss << value;
    return ss.str();
}

string StringUtils::toString(unsigned int value){
	ostringstream ss;
    ss << value;
    return ss.str();
}

// Para manejo de char* *************************

// Busca un carater en una cadena a partir de una posicion, devuelve la posicion del caracter en la cadena (relativa a startPosition) o -1 si no esta.
int StringUtils::find(const char* data, int startPosition, int dataSize, char characterToLookFor){
	for(int i=startPosition; i < dataSize; i++){
		if(data[i] == characterToLookFor)
			return i - startPosition;
	}
	return -1;
}

// Busca un texto en una cadena a partir de una posicion, devuelve la posicion del caracter en la cadena (relativa a startPosition) o -1 si no esta.
int StringUtils::find(const char* data, int startPosition, int dataSize, char* textToLookFor){
	for(int i=startPosition; i + strlen(textToLookFor) <= dataSize; i++){
		bool matchFound = true;
		for(int pos=0; pos < strlen(textToLookFor); pos++){
			if(data[i+pos] != textToLookFor[pos]){
				matchFound = false;
			}
		}
		if(matchFound){
			return i-startPosition;
		}
	}
	return -1;
}

int StringUtils::find(string data, string textToLookFor){
	int findedIndex = data.find(textToLookFor);
	if (findedIndex == std::string::npos) {
		return -1;
	}
	return findedIndex;
}

bool StringUtils::contains(string source, string textToLookFor){
	return (source.find(textToLookFor) != std::string::npos);
}

void StringUtils::substring(char* destination, const char* source, int startPosition, int length){
	for(int i=0; i < length; i++){
		destination[i] = source[startPosition + i];			
	}
}

string StringUtils::substring(string source, int startPosition, int length){
	return source.substr(startPosition, length);
}
//***************************************************

string StringUtils::replaceAll(string source, char originalChar, char finalChar){
	std::replace(source.begin(), source.end(), originalChar, finalChar);
	return source;
}

string StringUtils::replaceAll(string source, char originalChar, string finalSrc){
	string finalString = "";
	const char* sourceStr = source.c_str();
	for(int i=0; i < source.size(); i++){
		if(sourceStr[i] == originalChar){
			finalString += finalSrc;
		}
		else{
			finalString += sourceStr[i];
		}
	}	
	return finalString;
}


//***************Encoding de parametros de urls para su envio via http **************
string httpEncodings[256];
void loadHttpEncodings(){
	static bool httpCodesLoaded = false;
	if(!httpCodesLoaded){
		httpEncodings[' '] = "%20";
		httpEncodings['<'] = "%3C";
		httpEncodings['>'] = "%3E";
		httpEncodings['#'] = "%23";
		httpEncodings['%'] = "%25";
		httpEncodings['{'] = "%7B";
		httpEncodings['}'] = "%7D";
		httpEncodings['|'] = "%7C";
		httpEncodings['\\'] = "%5C";
		httpEncodings['^'] = "%5E";
		httpEncodings['~'] = "%7E";
		httpEncodings['['] = "%5B";
		httpEncodings[']'] = "%5D";
		httpEncodings['`'] = "%60";
		httpEncodings[';'] = "%3B";
		httpEncodings['/'] = "%2F";
		httpEncodings['?'] = "%3F";
		httpEncodings[':'] = "%3A";
		httpEncodings['@'] = "%40";
		httpEncodings['='] = "%3D";
		httpEncodings['&'] = "%26";
		httpEncodings['$'] = "%24";
		httpCodesLoaded = true;
	}
}

string StringUtils::encodeUrlParameter(string plainParameter){
	loadHttpEncodings();
	string finalString = "";
	const char* sourceStr = plainParameter.c_str();
	for(int i=0; i < plainParameter.size(); i++){
		// Si existe encoding para el caracter, lo reemplazamos
		if(!httpEncodings[sourceStr[i]].empty()){
			finalString += httpEncodings[sourceStr[i]];
		}
		else{
			finalString += sourceStr[i];
		}
	}	
	return finalString;
}
//*********************************************************************

