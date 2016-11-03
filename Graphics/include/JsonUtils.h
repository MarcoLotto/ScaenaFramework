/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "DataConformationException.h"
#include <string>
using namespace rapidjson;
using namespace std;

class JsonUtils{
	
public:
	static bool parseJson(rapidjson::Document& jsonDestination, string fileContent);
	static bool parseJsonFromFile(rapidjson::Document& jsonDestination, const char* filename);
	static bool getJsonString(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, std::string& fieldValue);
	static bool getJsonInt(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, int* fieldValue);
	static bool getJsonArray(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, Value* jsonArray);
	static bool getJsonObject(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, Value* jsonObject);
	static bool getJsonFloat(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, float* fieldValue);

	//*** Mismos metodos que los anteriores, pero en vez de devolver un boleano con el resultado, lanzan una excepcion al fallar
	static void parseJsonWExc(rapidjson::Document& jsonDestination, string fileContent);
	static void parseJsonFromFileWExc(rapidjson::Document& jsonDestination, const char* filename);
	static void getJsonStringWExc(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, std::string& fieldValue);
	static void getJsonIntWExc(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, int* fieldValue);
	static void getJsonArrayWExc(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, Value* jsonArray);
	static void getJsonObjectWExc(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, Value* jsonObject);
	static void getJsonFloatWExc(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, float* fieldValue);
};
