/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "JsonUtils.h"
#include<iostream>
#include<fstream>
using namespace std;

bool JsonUtils::getJsonString(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, std::string& fieldValue){
	if(!jsonDocument.HasMember(fieldName) || !jsonDocument[fieldName].IsString()){ 
		return false;    	 
	}
	fieldValue = jsonDocument[fieldName].GetString();
	return true;
}

bool JsonUtils::getJsonInt(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, int* fieldValue){
	if(!jsonDocument.HasMember(fieldName) || !jsonDocument[fieldName].IsInt()){ 
		return false;    	 
	}
	(*fieldValue) = jsonDocument[fieldName].GetInt();
	return true;
}

bool JsonUtils::getJsonFloat(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, float* fieldValue){
	if(!jsonDocument.HasMember(fieldName) || (!jsonDocument[fieldName].IsDouble() && !jsonDocument[fieldName].IsInt())){ 
		return false;    	 
	}
	(*fieldValue) = jsonDocument[fieldName].GetDouble();
	return true;
}

bool JsonUtils::getJsonArray(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, Value* jsonArray){
	if(!jsonDocument.HasMember(fieldName) || !jsonDocument[fieldName].IsArray()){ 
		return false;    	 
	}
	(*jsonArray) = jsonDocument[fieldName];
	return true;
}

bool JsonUtils::getJsonObject(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, Value* jsonObject){
	if(!jsonDocument.HasMember(fieldName) || !jsonDocument[fieldName].IsObject()){ 
		return false;    	 
	}
	(*jsonObject) = jsonDocument[fieldName];
	return true;
}

bool JsonUtils::parseJson(rapidjson::Document& jsonDestination, string fileContent){	
	if (jsonDestination.Parse(fileContent.c_str()).HasParseError()){			
		return false;
	} 
	return true;
}

bool JsonUtils::parseJsonFromFile(rapidjson::Document& jsonDestination, const char* filename){	
	 ifstream myReadFile;
	 string output, aux;
	 myReadFile.open(filename);	 
	 if (myReadFile.is_open()) {
		 while (!myReadFile.eof()) {
			myReadFile >> aux;
			output += aux;
		 }
	}
	 else{
		 return false;
	 }
	myReadFile.close();
	return parseJson(jsonDestination, output);
}



void JsonUtils::parseJsonWExc(rapidjson::Document& jsonDestination, string fileContent){
	if(!parseJson(jsonDestination, fileContent))
		throw DataConformationException("Error durante el parseo de data json");
}

void JsonUtils::getJsonStringWExc(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, std::string& fieldValue){
	if(!getJsonString(jsonDocument, fieldName, fieldValue))
		throw DataConformationException("Error durante el procesado de data json");
}

void JsonUtils::getJsonIntWExc(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, int* fieldValue){
	if(!getJsonInt(jsonDocument, fieldName, fieldValue))
		throw DataConformationException("Error durante el procesado de data json");
}

void JsonUtils::getJsonFloatWExc(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, float* fieldValue){
	if(!getJsonFloat(jsonDocument, fieldName, fieldValue))
		throw DataConformationException("Error durante el procesado de data json");
}

void JsonUtils::getJsonArrayWExc(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, Value* jsonArray){
	if(!getJsonArray(jsonDocument, fieldName, jsonArray))
		throw DataConformationException("Error durante el procesado de data json");
}

void JsonUtils::getJsonObjectWExc(rapidjson::GenericValue<rapidjson::UTF8<> >& jsonDocument, const char* fieldName, Value* jsonObject){
	if(!getJsonObject(jsonDocument, fieldName, jsonObject))
		throw DataConformationException("Error durante el procesado de data json");
}

void JsonUtils::parseJsonFromFileWExc(rapidjson::Document& jsonDestination, const char* filename){
	if(!parseJsonFromFile(jsonDestination, filename))
		throw DataConformationException("Error durante el parseo del archivo json " + string(filename));
}