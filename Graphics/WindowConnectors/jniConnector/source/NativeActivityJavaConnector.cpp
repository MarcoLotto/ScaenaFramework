/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "TestProject", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "TestProject", __VA_ARGS__))

#include "StringUtils.h"
#include <string>
using namespace std;

// Guardamos el state de android de la aplicación
android_app* androidState;

void saveAndroidStateOnNativeConnector(android_app* state){
	androidState = state;
}

/****************************************************
* Llamados a funciones java desde ndk
*****************************************************/
bool prepareForCallJavaMethod(ANativeActivity** activity, JavaVM** jvm, JNIEnv** env){	   	
	(*activity) = androidState->activity;
    (*jvm) = androidState->activity->vm;
    (*jvm)->GetEnv((void **)env, JNI_VERSION_1_6);
    jint res = (*jvm)->AttachCurrentThread(env, NULL);
    if (res == JNI_ERR)
    {
	  __android_log_print(ANDROID_LOG_ERROR, "MainNativeActivity#prepareForCallJavaMethod", "Error al traer el JVM enviroment");
        return false; 
    } 
	return true;
}

/********* RETORNO VOID *****************/

void callVoidJavaMethod(const char* javaMethodName){
	ANativeActivity* activity = NULL; JavaVM* jvm = NULL; JNIEnv* env = NULL;	
	if(prepareForCallJavaMethod(&activity, &jvm, &env)){
		jclass clazz = env->GetObjectClass(activity->clazz);
		jmethodID methodID = env->GetMethodID(clazz, javaMethodName, "()V");
		env->CallVoidMethod(activity->clazz, methodID);
		jvm->DetachCurrentThread();
	}
}
void callVoidJavaMethod(const char* javaMethodName, int param){
	ANativeActivity* activity = NULL; JavaVM* jvm = NULL; JNIEnv* env = NULL;	
	if(prepareForCallJavaMethod(&activity, &jvm, &env)){
		jclass clazz = env->GetObjectClass(activity->clazz);
		jmethodID methodID = env->GetMethodID(clazz, javaMethodName, "(I)V");
		env->CallVoidMethod(activity->clazz, methodID, param);
		jvm->DetachCurrentThread();
	}
}
void callVoidJavaMethod(const char* javaMethodName, string param){
	ANativeActivity* activity = NULL; JavaVM* jvm = NULL; JNIEnv* env = NULL;	
	if(prepareForCallJavaMethod(&activity, &jvm, &env)){
		jclass clazz = env->GetObjectClass(activity->clazz);
		jmethodID methodID = env->GetMethodID(clazz, javaMethodName, "(Ljava/lang/String;)V");
		jstring jStringParam = env->NewStringUTF( param.c_str() );
		if( !jStringParam )
		{ 
			__android_log_print(ANDROID_LOG_ERROR, "MainNativeActivity#callJavaMethod", "Fallo al pasar string de ndk a java");
			return;
		}
		env->CallVoidMethod(activity->clazz, methodID, jStringParam);
		env->DeleteLocalRef( jStringParam );
		jvm->DetachCurrentThread();
	}
}

/********* RETORNO DE INT *****************/

int callIntJavaMethod(const char* javaMethodName){
	ANativeActivity* activity = NULL; JavaVM* jvm = NULL; JNIEnv* env = NULL;	
	int result = -1;
	if(prepareForCallJavaMethod(&activity, &jvm, &env)){
		jclass clazz = env->GetObjectClass(activity->clazz);
		jmethodID methodID = env->GetMethodID(clazz, javaMethodName, "()I");
		result = env->CallIntMethod(activity->clazz, methodID);
		jvm->DetachCurrentThread();
	}
	return result;
}
int callIntJavaMethod(const char* javaMethodName, int param){
	ANativeActivity* activity = NULL; JavaVM* jvm = NULL; JNIEnv* env = NULL;	
	int result = -1;
	if(prepareForCallJavaMethod(&activity, &jvm, &env)){
		jclass clazz = env->GetObjectClass(activity->clazz);
		jmethodID methodID = env->GetMethodID(clazz, javaMethodName, "(I)I");
		result = env->CallIntMethod(activity->clazz, methodID, param);
		jvm->DetachCurrentThread();
	}
	return result;
}
int callIntJavaMethod(const char* javaMethodName, string param){
	ANativeActivity* activity = NULL; JavaVM* jvm = NULL; JNIEnv* env = NULL;	
	int result = -1;
	if(prepareForCallJavaMethod(&activity, &jvm, &env)){
		jclass clazz = env->GetObjectClass(activity->clazz);
		jmethodID methodID = env->GetMethodID(clazz, javaMethodName, "(Ljava/lang/String;)I");
		jstring jStringParam = env->NewStringUTF( param.c_str() );
		if( !jStringParam )
		{ 
			__android_log_print(ANDROID_LOG_ERROR, "MainNativeActivity#callJavaMethod", "Fallo al pasar string de ndk a java");
			return result;
		}
		result = env->CallIntMethod(activity->clazz, methodID, jStringParam);
		env->DeleteLocalRef( jStringParam );
		jvm->DetachCurrentThread();
	}
	return result;
}

/********* RETORNO DE STRING *****************/

string jstringToString(jstring jstringValue, JNIEnv* env){
	const char *js = env->GetStringUTFChars(jstringValue, NULL);
    std::string cs(js);
    env->ReleaseStringUTFChars(jstringValue, js);
    return cs;
}

string callStringJavaMethod(const char* javaMethodName){
	ANativeActivity* activity = NULL; JavaVM* jvm = NULL; JNIEnv* env = NULL;	
	string result;
	if(prepareForCallJavaMethod(&activity, &jvm, &env)){
		jclass clazz = env->GetObjectClass(activity->clazz);
		jmethodID methodID = env->GetMethodID(clazz, javaMethodName, "()Ljava/lang/String;");
		jstring jresult = (jstring) env->CallObjectMethod(activity->clazz, methodID);
		result = jstringToString(jresult, env);
		jvm->DetachCurrentThread();		
	}	
	return result;
}
string callStringJavaMethod(const char* javaMethodName, int param){
	ANativeActivity* activity = NULL; JavaVM* jvm = NULL; JNIEnv* env = NULL;	
	string result;
	if(prepareForCallJavaMethod(&activity, &jvm, &env)){
		jclass clazz = env->GetObjectClass(activity->clazz);
		jmethodID methodID = env->GetMethodID(clazz, javaMethodName, "(I)Ljava/lang/String;");
		jstring jresult = (jstring) env->CallObjectMethod(activity->clazz, methodID, param);
		result = jstringToString(jresult, env);
		jvm->DetachCurrentThread();
	}
	return result;
}
string callStringJavaMethod(const char* javaMethodName, string param){
	ANativeActivity* activity = NULL; JavaVM* jvm = NULL; JNIEnv* env = NULL;	
	string result;
	if(prepareForCallJavaMethod(&activity, &jvm, &env)){
		jclass clazz = env->GetObjectClass(activity->clazz);
		jmethodID methodID = env->GetMethodID(clazz, javaMethodName, "(Ljava/lang/String;)Ljava/lang/String;");
		jstring jStringParam = env->NewStringUTF( param.c_str() );
		if( !jStringParam )
		{ 
			__android_log_print(ANDROID_LOG_ERROR, "NativeActivityConnector#callStringJavaMethod", "Fallo al pasar string de ndk a java");
			return result;
		}
		jstring jresult = (jstring) env->CallObjectMethod(activity->clazz, methodID, jStringParam);
		result = jstringToString(jresult, env);
		env->DeleteLocalRef( jStringParam );
		jvm->DetachCurrentThread();
	}
	return result;
}
string callStringJavaMethod(const char* javaMethodName, string param1, string param2, string param3){
	ANativeActivity* activity = NULL; JavaVM* jvm = NULL; JNIEnv* env = NULL;	
	string result;
	if(prepareForCallJavaMethod(&activity, &jvm, &env)){
		jclass clazz = env->GetObjectClass(activity->clazz);
		jmethodID methodID = env->GetMethodID(clazz, javaMethodName, "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
		jstring jStringParam1 = env->NewStringUTF( param1.c_str() );
		jstring jStringParam2 = env->NewStringUTF( param2.c_str() );
		jstring jStringParam3 = env->NewStringUTF( param3.c_str() );
		if( !jStringParam1 || !jStringParam2 || !jStringParam3 )
		{ 
			__android_log_print(ANDROID_LOG_ERROR, "NativeActivityConnector#callStringJavaMethod", "Fallo al pasar string de ndk a java");
			return result;
		}
		jstring jresult = (jstring) env->CallObjectMethod(activity->clazz, methodID, jStringParam1, jStringParam2, jStringParam3);
		result = jstringToString(jresult, env);
		env->DeleteLocalRef( jStringParam1 );
		env->DeleteLocalRef( jStringParam2 );
		env->DeleteLocalRef( jStringParam3 );
		jvm->DetachCurrentThread();
	}
	return result;
}
string callStringJavaMethod(const char* javaMethodName, string param1, string param2, string param3, string param4){
	ANativeActivity* activity = NULL; JavaVM* jvm = NULL; JNIEnv* env = NULL;	
	string result;
	if(prepareForCallJavaMethod(&activity, &jvm, &env)){
		jclass clazz = env->GetObjectClass(activity->clazz);
		jmethodID methodID = env->GetMethodID(clazz, javaMethodName, "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
		jstring jStringParam1 = env->NewStringUTF( param1.c_str() );
		jstring jStringParam2 = env->NewStringUTF( param2.c_str() );
		jstring jStringParam3 = env->NewStringUTF( param3.c_str() );
		jstring jStringParam4 = env->NewStringUTF( param4.c_str() );
		if( !jStringParam1 || !jStringParam2 || !jStringParam3 || !jStringParam4 )
		{ 
			__android_log_print(ANDROID_LOG_ERROR, "NativeActivityConnector#callStringJavaMethod", "Fallo al pasar string de ndk a java");
			return result;
		}
		jstring jresult = (jstring) env->CallObjectMethod(activity->clazz, methodID, jStringParam1, jStringParam2, jStringParam3, jStringParam4);
		result = jstringToString(jresult, env);
		env->DeleteLocalRef( jStringParam1 );
		env->DeleteLocalRef( jStringParam2 );
		env->DeleteLocalRef( jStringParam3 );
		env->DeleteLocalRef( jStringParam4 );
		jvm->DetachCurrentThread();
	}
	return result;
}
