/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "FbxModelLoader.h"
#include "ScaenaModelWriter.h"
#include "ScaenaAnimationWriter.h"
#include "KeyFrameDataTransfer.h"
#include "Logger.h"
#include "XmlParser.h"
#include "GenerationProcessor.h"

bool getAnimationParams(int argc, char* argv[], string& inAnimationFile, string& outAnimationFile, string& bindPoseMeshFile, string&  keyFrameFBXFile, string& animationName, float& animationTime);

//*******************************************************************************************************************
// Esta aplicacion se utiliza para transformar modelos de diferentes formatos (ej, FBX) a Scaena Model File (.smf).
// Modo de uso: Correr el programa pasando como parametro el archivo a convertir a .smf
//*******************************************************************************************************************

int main(int argc, char* argv[])
{
	// Inicializo el logger
	Logger::getInstance("./generalLog.txt", "./errorLog.txt", true, true, true); 

	char inputText[300];
	inputText[0] = 0;
	printf("//******************************************************//\n");
	printf("//------------SCAENA MODEL TRANSFORMER------------------//\n");
	printf("//------------------Marco.A.Lotto-----------------------//\n");
	printf("//******************************************************//\n\n");
	while(inputText[0] != '1' && inputText[0] != '2'){
		printf("1. Convertir modelo de FBX a SMF\n");
		printf("2. Generar animacion de FBX a SAF\n");
		printf("   Que desea hacer? (1/2): "); 
		scanf("%s", inputText);
		printf("//******************************************************//\n");
	}
	if(inputText[0] == '1'){	// Proceso Scaena Model File (.SMF)

		printf("Indique el nombre del archivo FBX de entrada: ");
		scanf("%s", inputText);
		printf("//******************************************************//\n");
		// Convertimos
		if(!GenerationProcessor::processScaenaModelFile(inputText)){
			return 1;  // Termino mal
		}		
	}
	else{	// Proceso Scaena Animation File (.SAF)

		string inAnimationFile, outAnimationFile, keyFrameFBXFile, animationName;
		while(inputText[0] != 'S' && inputText[0] != 'N'){
			printf("Desea agregar informacion en un archivo SAF existente? (S/N): ");
			scanf("%s", inputText);		
			printf("//******************************************************//\n");
		}
		if(inputText[0] == 'S'){
			printf("Indique el nombre del archivo SMF a modificar: ");
			scanf("%s", inputText);
			printf("//******************************************************//\n");
			inAnimationFile = string(inputText);
			outAnimationFile = inAnimationFile;
		}
		else{
			printf("Indique el nombre del archivo SMF de salida: ");
			scanf("%s", inputText);
			printf("//******************************************************//\n");
			outAnimationFile = string(inputText);
		}
		while(animationName.compare("N") != 0){
			printf("Indique el nombre de la animacion a crear/modificar (N para terminar): ");
			scanf("%s", inputText);		
			printf("//******************************************************//\n");
			animationName = string(inputText);
			keyFrameFBXFile = "";

			while(keyFrameFBXFile.compare("N") != 0 && animationName.compare("N") != 0){
				printf("Indique el nombre del archivo FBX con la pose para agregar un keyframe (N para terminar): ");
				scanf("%s", inputText);		
				printf("//******************************************************//\n");
				keyFrameFBXFile = string(inputText);

				if(keyFrameFBXFile.compare("N") != 0){
					float duration = 0.0f;
					printf("Indique la duracion del keyframe en segundos: ");
					scanf("%s", inputText);		
					printf("//******************************************************//\n");
					duration = atof(inputText);
					// Convertimos
					if(!GenerationProcessor::processScaenaAnimationFile(inAnimationFile, outAnimationFile, keyFrameFBXFile, animationName, duration)){
						return 1;  // Termino mal
					}
					printf("//******************************************************//\n");
					printf("//KEYFRAME AGREGADO A LA ANIMACION: %s\n", animationName.c_str());
					printf("//******************************************************//\n");
				}				
			}
			if(animationName.compare("N") != 0){
				printf("//******************************************************//\n");
				printf("//ANIMACION %s PROCESADA CORRECTAMENTE\n", animationName.c_str());
				printf("//******************************************************//\n");
			}
		}
	}	
	scanf("Presione enter para finalizar");  
	return 0;
}

bool getAnimationParams(int argc, char* argv[], string& inAnimationFile, string& outAnimationFile, string& bindPoseMeshFile, string&  keyFrameFBXFile, string& animationName, float& animationTime){
	if(argc != 9 && argc != 11){  
		Logger::getInstance()->logError(new Log("Cantidad de parametros incorrectos para la animacion"));
		return false;
	}
	else if(argc == 11 && strcmp(argv[1], "-a")!=0 && strcmp(argv[3], "-in")!=0 && strcmp(argv[5], "-out")!=0 && strcmp(argv[7], "-kf")!=0){  
		Logger::getInstance()->logError(new Log("No se define algun parametro necesario para animacion o estan en orden incorrecto. Recuerde que son [-a, -in, -out, -kf] en ese orden."));
		return false;
	}
	else if(argc == 9 && strcmp(argv[1], "-a")!=0 && strcmp(argv[3], "-out")!=0 && strcmp(argv[5], "-kf")!=0){  
		Logger::getInstance()->logError(new Log("No se define algun parametro necesario para animacion o estan en orden incorrecto. Recuerde que son [-a, -out, -kf] en ese orden."));
		return false;
	}
	// Ya hice las validaciones, me guardo los parametros
	if(argc == 11){
		bindPoseMeshFile = string(argv[2]);
		inAnimationFile = string(argv[4]);
		outAnimationFile = string(argv[6]);
		keyFrameFBXFile = string(argv[8]);
		animationName = string(argv[9]);
		animationTime = atof(argv[10]);
	}
	else if(argc == 9){
		bindPoseMeshFile = string(argv[2]);
		inAnimationFile = "";
		outAnimationFile = string(argv[4]);
		keyFrameFBXFile = string(argv[6]);
		animationName = string(argv[7]);
		animationTime = atof(argv[8]);
	}	
	return true;

}

