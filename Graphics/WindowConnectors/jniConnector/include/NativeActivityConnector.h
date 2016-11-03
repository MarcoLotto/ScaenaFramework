/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <android_native_app_glue.h>

// Llamar al crearse el programa para guardar el estado de la app android
void saveAndroidStateOnNativeConnector(android_app* state);

/****************************************************
* Llamados a funciones java desde ndk
*****************************************************/
void callVoidJavaMethod(const char* javaMethodName);
void callVoidJavaMethod(const char* javaMethodName, int param);
void callVoidJavaMethod(const char* javaMethodName, string param);

int callIntJavaMethod(const char* javaMethodName);
int callIntJavaMethod(const char* javaMethodName, int param);
int callIntJavaMethod(const char* javaMethodName, string param);

string callStringJavaMethod(const char* javaMethodName);
string callStringJavaMethod(const char* javaMethodName, int param);
string callStringJavaMethod(const char* javaMethodName, string param);
string callStringJavaMethod(const char* javaMethodName, string param1, string param2, string param3);
string callStringJavaMethod(const char* javaMethodName, string param1, string param2, string param3, string param4);