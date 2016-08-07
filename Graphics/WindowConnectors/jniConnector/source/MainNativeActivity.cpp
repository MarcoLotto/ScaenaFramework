/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

//BEGIN_INCLUDE(all)
#include "MainNativeActivity.h"
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

#include "TimeManager.h"
#include "myWindow.h"
#include "GraphicDevice.h"
#include "TouchEvent.h"
#include "NativeActivityConnector.h"

#include "StringUtils.h"
#include "JsonUtils.h"
#include <string>
using namespace std;

#define APP_VERSION_FILENAME "appVersion.json"
#define APP_VERSION_AUX_FILENAME "appVersionAux.json"

myWindow* windowConnector = NULL;

/**
 * Our saved state data.
 */
struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};

/**
 * Shared state for our app.
 */
struct engine {
    struct android_app* app;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
	EGLConfig config;
    int32_t width;
    int32_t height;
    struct saved_state state;
};
// Contiene la informacion de estado de la app android
struct engine engine;

// Data del acelerometro
float acelerometerX, acelerometerY, acelerometerZ;

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(struct engine* engine) {
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows (Y SE UTILIZA OPENGL ES 3)
     */
	 const EGLint attribs[] = {
	    	EGL_SURFACE_TYPE,
	    	EGL_WINDOW_BIT,
	    	EGL_BLUE_SIZE, 8,
	    	EGL_GREEN_SIZE, 8,
	        EGL_RED_SIZE, 8,
	        EGL_DEPTH_SIZE, 24,
	    	EGL_NONE
	};

    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);
    assert(eglGetError() == EGL_SUCCESS);
    eglBindAPI(EGL_OPENGL_API);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    assert(eglGetError() == EGL_SUCCESS);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);

    //****Primero pruebo crear un contexto de opengl ES 3.0, si falla creo un contexto con opengl ES 2.0
    static const EGLint ctx_attribs_GLES3[] = {
         EGL_CONTEXT_CLIENT_VERSION, 3,
         EGL_NONE
    };
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctx_attribs_GLES3);
	if(context != NULL){
		GraphicDevice::bindApi(OPENGLES_3_API);	 // Le informo a Scaena que version de opengl voy a usar
		__android_log_print(ANDROID_LOG_ERROR, "OPENGLES VERSION", "3");
	}
	else{
		__android_log_print(ANDROID_LOG_ERROR, "OPENGLES VERSION", "2");
		static const EGLint ctx_attribs_GLES2[] = {
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
		};
		context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctx_attribs_GLES2);
		GraphicDevice::bindApi(OPENGLES_2_API);
	}
	//*******************************************************************************************************

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
	engine->config = config;
    engine->width = w;
    engine->height = h;
    engine->state.angle = 0;

    //Creo el conector de Scaena con la ventana
    windowConnector = new myWindow();
    windowConnector->OnInit();
    windowConnector->OnResize((int) engine->width, (int) engine->height);

    return 0;
}

void swapBuffers(){
	eglSwapBuffers(engine.display, engine.surface);
}

/**
 * Just the current frame in the display.
 */
static void engine_draw_frame(struct engine* engine) {
    if (engine->display == NULL) {
        // No display.
        return;
    }
    // Le paso el control a scaena
    windowConnector->OnUpdate();
    windowConnector->OnRender();
}

void validateAndCreateDirectory(struct android_app* state, string directory){
	ANativeActivity* nativeActivity = state->activity;
	const char* internalPath = nativeActivity->internalDataPath;
	std::string dataPath(internalPath);
	dataPath +=  "/" + directory;

	struct stat sb;
	int res = stat(dataPath.c_str(), &sb);
	if (0 == res && sb.st_mode & S_IFDIR)
	{
		__android_log_print(ANDROID_LOG_ERROR, "INFO", (directory + "/ dir already in app's internal data storage.").c_str());
	}
	else if (ENOENT == errno)
	{
		res = mkdir(dataPath.c_str(), 0770);
	}
}

// Como en c++ no puedo tener acceso a la carpeta assets sin el asset manager, copio los datos a files
void copyAssetDirectoryToProgramDirectory(struct android_app* state, string directory, bool forceFileOverride){
	ANativeActivity* nativeActivity = state->activity;
	const char* internalPath = nativeActivity->internalDataPath;
	std::string dataPath(internalPath);

	AAssetDir* assetDirectory = AAssetManager_openDir(nativeActivity->assetManager, directory.c_str());
	if(assetDirectory == NULL){
		__android_log_print(ANDROID_LOG_ERROR, "ERROR", "Directory does not exist in assetsManager folder");
		return;
	}
	AAssetDir_rewind(assetDirectory);
	const char* file = AAssetDir_getNextFileName(assetDirectory);
	while(file != NULL){
		__android_log_print(ANDROID_LOG_ERROR, "FILE", ("Copying to internal storange: " + directory + "/" + file).c_str());

		// Path del archivo en memoria accesible por Scaena
		std::string configFile = dataPath + "/" + directory + "/" + file;

		// Verificamos si ya existe el archivo en memoria accesible por Scaena, de no ser asi, lo copiamos
    struct stat sb;
		int res = stat(configFile.c_str(), &sb);
		if ( (0 == res && sb.st_mode & S_IFREG) && !forceFileOverride)
		{
			__android_log_print(ANDROID_LOG_ERROR, "INFO", "Application file already present");
		}
		else
		{
			__android_log_print(ANDROID_LOG_ERROR, "INFO", "Application file does not exist. Creating it ...");
			// read our application config file from the assets inside the apk
			// save the config file contents in the application's internal storage
			__android_log_print(ANDROID_LOG_ERROR, "INFO", "Reading file using the asset manager.\n");

			AAssetManager* assetManager = nativeActivity->assetManager;
			AAsset* configFileAsset = AAssetManager_open(assetManager, (directory + "/" + file).c_str(), AASSET_MODE_BUFFER);
			const void* configData = AAsset_getBuffer(configFileAsset);
			const off_t configLen = AAsset_getLength(configFileAsset);
			FILE* appConfigFile = std::fopen(configFile.c_str(), "w+");
			if (NULL == appConfigFile)
			{
				__android_log_print(ANDROID_LOG_ERROR, "INFO", "Could not create app internal file.");
			}
			else
			{
				__android_log_print(ANDROID_LOG_ERROR, "INFO", "App file created successfully. Writing data ...");
				res = std::fwrite(configData, sizeof(char), configLen, appConfigFile);
				if (configLen != res)
				{
					__android_log_print(ANDROID_LOG_ERROR, "INFO", "Error generating app file.");
				}
			}
			std::fclose(appConfigFile);
			AAsset_close(configFileAsset);
		}
		file = AAssetDir_getNextFileName(assetDirectory);
	}
	AAssetDir_close(assetDirectory);
}

bool copyAppVersionToScaenaAppStorange(struct android_app* state, string directory){
  ANativeActivity* nativeActivity = state->activity;
	const char* internalPath = nativeActivity->internalDataPath;
	std::string dataPath(internalPath);

	AAssetDir* assetDirectory = AAssetManager_openDir(nativeActivity->assetManager, directory.c_str());
	if(assetDirectory == NULL){
		__android_log_print(ANDROID_LOG_ERROR, "ERROR", "No existe el directorio de appVersion en el assetManager");
		return false;
	}
	AAssetDir_rewind(assetDirectory);
	const char* file = AAssetDir_getNextFileName(assetDirectory);
	while(file != NULL){
    if(string(file).compare(APP_VERSION_FILENAME)==0){
      AAssetManager* assetManager = nativeActivity->assetManager;
      AAsset* configFileAsset = AAssetManager_open(assetManager, (directory + "/" + file).c_str(), AASSET_MODE_BUFFER);
      const void* appVersionData = AAsset_getBuffer(configFileAsset);
			const off_t appVersionLength = AAsset_getLength(configFileAsset);

      std::string appVersionAuxFilename = dataPath + "/" + directory + "/" + APP_VERSION_AUX_FILENAME;
			FILE* appVersionAuxFile = std::fopen(appVersionAuxFilename.c_str(), "w+");
			if (NULL == appVersionAuxFile)
			{
				__android_log_print(ANDROID_LOG_ERROR, "ERROR", "Could not create AppVersionAux internal file.");
        return false;
			}
			else
			{
  				int res = std::fwrite(appVersionData, sizeof(char), appVersionLength, appVersionAuxFile);
  				if (appVersionLength != res)
  				{
  					__android_log_print(ANDROID_LOG_ERROR, "ERROR", "Error generating AppVersionAux file.");
            return false;
  				}
          std::fclose(appVersionAuxFile);
          AAsset_close(configFileAsset);
          AAssetDir_close(assetDirectory);
          return true;
			}
    }
		file = AAssetDir_getNextFileName(assetDirectory);
	}
	AAssetDir_close(assetDirectory);
  __android_log_print(ANDROID_LOG_ERROR, "ERROR", "AppVersion file could not be found on AssetManager");
  return false;
}

// Comprueba si se hizo una actualizacion de la aplicacion (si hay errores en los archivos de version, se toma como que hubo un update)
bool verifyAppVersion(struct android_app* state, string directory){
    // Primero copiamos un auxiliar del appVersion.json en la memoria accesible por Scaena
    if(!copyAppVersionToScaenaAppStorange(state, directory)){
        __android_log_print(ANDROID_LOG_ERROR, "INFO", "AppVersionAux writing error");
        return true;
    }
    ANativeActivity* nativeActivity = state->activity;
  	string dataPath = string(nativeActivity->internalDataPath);
    string finalDirectory = dataPath + "/" + directory + "/";

    // Ahora comparamos la version auxiliar que copiamos, con nuestra version del appVersion.json
    Document appVersionJson, appVersionAuxJson;
    if(!JsonUtils::parseJsonFromFile(appVersionJson, string(finalDirectory + APP_VERSION_FILENAME).c_str())){
      // No existe el appVersion, quiere decir que "probablemente" sea la primera vez que ejecutamos la app
      // TODO: Informamos a Scaena que es la primera vez que ejecutamos la app
      __android_log_print(ANDROID_LOG_ERROR, "INFO", "AppVersion not found. App is executed for the first time");
      return true;
    }
    if(!JsonUtils::parseJsonFromFile(appVersionAuxJson, string(finalDirectory + APP_VERSION_AUX_FILENAME).c_str())){
      __android_log_print(ANDROID_LOG_ERROR, "ERROR", "AppVersionAux file could not be found on App Storage (Bad copy?)");
      return true;
    }
    string installedAppVersion, updatedAppVersion;
    if(!JsonUtils::getJsonString(appVersionJson, "version", installedAppVersion) || !JsonUtils::getJsonString(appVersionAuxJson, "version", updatedAppVersion)){
      __android_log_print(ANDROID_LOG_ERROR, "ERROR", "Corrupted AppVersion (local or external)");
      return true;
    }
    __android_log_print(ANDROID_LOG_ERROR, "INFO", ("Installed AppVersion: " + installedAppVersion + ", update AppVersion: " + updatedAppVersion).c_str());
    return (installedAppVersion.compare(updatedAppVersion) != 0);
}

// Borra todo el contexto de opengl y Scaena
static void engine_term_display(struct engine* engine) {
    if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT) {
            eglDestroyContext(engine->display, engine->context);
        }
        if (engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display, engine->surface);
        }
       eglTerminate(engine->display);
    }
    engine->animating = 0;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;

	// Le indico a Scaena que estoy cerrando
	windowConnector->Close();
}

// Desvincula la surface del contexto y del display, y luego la destruye para que no elimine automaticamente al contexto
static void engine_pause_display(struct engine* engine) {
    if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display, engine->surface);
        }
    }
    engine->animating = 0;
    engine->surface = EGL_NO_SURFACE;
}

// Crea nuevamente la surface y la vincula al contexto y display ya existentes
static void engine_resume_display(struct engine* engine) {
    engine->surface = eglCreateWindowSurface(engine->display, engine->config, engine->app->window, NULL);
	if (eglMakeCurrent(engine->display, engine->surface, engine->surface, engine->context) == EGL_FALSE) {
		LOGW("Unable to eglMakeCurrent");
		exit(0);
	}
}


/**
 * Hace una copia del directorio de assets al directorio local del programa de donde puedo leer data
 */
static void copyFilesFromAssetDirToProgramDir(struct android_app* app){
    // Creo las carpetas en el directorio local del programa
    validateAndCreateDirectory(app, "");
    validateAndCreateDirectory(app, "Definitions");
    validateAndCreateDirectory(app, "Definitions/Scenes");
    validateAndCreateDirectory(app, "Definitions/Objects");
    validateAndCreateDirectory(app, "Modelos");
    validateAndCreateDirectory(app, "Shaders");
    validateAndCreateDirectory(app, "Textures");
    validateAndCreateDirectory(app, "Textures/Difusas");
    validateAndCreateDirectory(app, "Textures/DisplacementMaps");
    validateAndCreateDirectory(app, "Textures/NormalMaps");
	validateAndCreateDirectory(app, "Textures/UI");
	validateAndCreateDirectory(app, "ProgramData");
	validateAndCreateDirectory(app, "Sounds");

    // Verifico si se produjo una actualizacion de la aplicacion
    bool appHasBeenUpdated = verifyAppVersion(app, "ProgramData");

    // Copio toda la carpeta de assets al directorio local del programa
    copyAssetDirectoryToProgramDirectory(app, "Definitions/Scenes", appHasBeenUpdated);
    copyAssetDirectoryToProgramDirectory(app, "Definitions/Objects", appHasBeenUpdated);
    copyAssetDirectoryToProgramDirectory(app, "Modelos", appHasBeenUpdated);
    copyAssetDirectoryToProgramDirectory(app, "Shaders", appHasBeenUpdated);
    copyAssetDirectoryToProgramDirectory(app, "Textures/Difusas", appHasBeenUpdated);
    copyAssetDirectoryToProgramDirectory(app, "Textures/DisplacementMaps", appHasBeenUpdated);
    copyAssetDirectoryToProgramDirectory(app, "Textures/NormalMaps", appHasBeenUpdated);
	copyAssetDirectoryToProgramDirectory(app, "Textures/UI", appHasBeenUpdated);
	copyAssetDirectoryToProgramDirectory(app, "ProgramData", appHasBeenUpdated);
	copyAssetDirectoryToProgramDirectory(app, "Sounds", appHasBeenUpdated);
}

int module(int value){
	return value * (-1*(value < 0) + (value >= 0));
}

static bool getAsciiCharacterRepresentation(int keyAction, int unicodeValue, char* asciiValue){
	// En android se informa el shift para mayuscula como una tecla separada, me lo guardo si viene
	static bool savedMayusActive = false;
	bool mayusActive = savedMayusActive;
	if(unicodeValue == AKEYCODE_SHIFT_LEFT || unicodeValue == AKEYCODE_SHIFT_RIGHT){
		if(keyAction == AKEY_EVENT_ACTION_DOWN){
			savedMayusActive = true;
		}
		return false;
	}
	if(keyAction == AKEY_EVENT_ACTION_DOWN){
		return false;
	}
	savedMayusActive = false;

	// Vamos ahora a mapear la tecla a ascii (solo algunos codigos ascii)
	if(unicodeValue >= AKEYCODE_A && unicodeValue <= AKEYCODE_Z){
		(*asciiValue) = unicodeValue + 68 - 32 * mayusActive;
		return true;
	}
	if(unicodeValue == AKEYCODE_1 && mayusActive){ (*asciiValue) = '!'; return true;}
	if(unicodeValue == AKEYCODE_2 && mayusActive){ (*asciiValue) = '@'; return true;}
	if(unicodeValue == AKEYCODE_3 && mayusActive){ (*asciiValue) = '#'; return true;}
	if(unicodeValue == AKEYCODE_4 && mayusActive){ (*asciiValue) = '$'; return true;}
	if(unicodeValue == AKEYCODE_5 && mayusActive){ (*asciiValue) = '}'; return true;}  // review
	if(unicodeValue == AKEYCODE_6 && mayusActive){ (*asciiValue) = '{'; return true;}  // review
	if(unicodeValue == AKEYCODE_7 && mayusActive){ (*asciiValue) = '&'; return true;}
	if(unicodeValue == AKEYCODE_8 && mayusActive){ (*asciiValue) = ']'; return true;}  // review
	if(unicodeValue == AKEYCODE_9 && mayusActive){ (*asciiValue) = '('; return true;}
	if(unicodeValue == AKEYCODE_0 && mayusActive){ (*asciiValue) = ')'; return true;}
	if(unicodeValue >= AKEYCODE_0 && unicodeValue <= AKEYCODE_9){
		(*asciiValue) = unicodeValue + 41;
		return true;
	}
	if(unicodeValue == AKEYCODE_MINUS && mayusActive){ (*asciiValue) = '_'; return true;}
	if(unicodeValue == AKEYCODE_SLASH && mayusActive){ (*asciiValue) = '?'; return true;}
	if(unicodeValue == AKEYCODE_APOSTROPHE && mayusActive){ (*asciiValue) = '"'; return true;}
	if(unicodeValue == AKEYCODE_SEMICOLON && mayusActive){ (*asciiValue) = ';'; return true;}
	if(unicodeValue == AKEYCODE_SPACE){ (*asciiValue) = ' '; return true;}
	if(unicodeValue == AKEYCODE_TAB){ (*asciiValue) = '	'; return true;}
	if(unicodeValue == AKEYCODE_ENTER){ (*asciiValue) = 13; return true;}
	if(unicodeValue == AKEYCODE_DEL){ (*asciiValue) = 8; return true;}
	if(unicodeValue == AKEYCODE_COMMA){ (*asciiValue) = ','; return true;}
	if(unicodeValue == AKEYCODE_PERIOD){ (*asciiValue) = '.'; return true;}
	if(unicodeValue == AKEYCODE_MINUS){ (*asciiValue) = '-'; return true;}
	if(unicodeValue == AKEYCODE_APOSTROPHE){ (*asciiValue) = '\''; return true;}
	if(unicodeValue == AKEYCODE_SEMICOLON){ (*asciiValue) = ':'; return true;}

	// No encontramos la tecla, no es una tecla valida para scaena
	return false;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    const int32_t type = AInputEvent_getType(event);
    if (AINPUT_EVENT_TYPE_MOTION == type) {

        int32_t action = AMotionEvent_getAction(event);
        uint32_t flags = action & AMOTION_EVENT_ACTION_MASK;
        int32_t pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

        // Convierto el tipo de toque particular de android a un toque generico para Scaena
        TouchType touchType;
        switch (flags) {
            case AMOTION_EVENT_ACTION_DOWN:
            case AMOTION_EVENT_ACTION_POINTER_DOWN:
                touchType = TouchType::began;
                break;
            case AMOTION_EVENT_ACTION_UP:
            case AMOTION_EVENT_ACTION_POINTER_UP:
                touchType = TouchType::ended;
                break;
            case AMOTION_EVENT_ACTION_MOVE:
                touchType = TouchType::moved;
                break;
            case AMOTION_EVENT_ACTION_CANCEL:
                touchType = TouchType::cancelled;
                break;
            default:
                return 0;
        }
		// Genero el touchEvent y lo lleno
		TouchEvent touchEvent(touchType, WindowConnector::GetOSCurrentTime());
        unsigned int touchCount = AMotionEvent_getPointerCount(event);
        for (unsigned int i = 0; i < touchCount; i++) {
			int xPos = module(AMotionEvent_getX(event, i));
			int yPos = module(AMotionEvent_getY(event, i));
			int pointId = AMotionEvent_getPointerId(event, i);
			bool hasChanged = (i == pointerIndex);
			TouchPoint* touchPoint = new TouchPoint(xPos, yPos, pointId, hasChanged);
			touchEvent.addTouchPoint(touchPoint);

			// Si es el primer toque, tambien lo informe como toque del mouse (por ejemplo por si toca la user interface)
			if(i == 0){
				if(touchType == TouchType::began)
					windowConnector->OnMouseDown(0, xPos, yPos);
				else if(touchType == TouchType::ended)
					windowConnector->OnMouseUp(0, xPos, yPos);
				else
					windowConnector->OnLeftMouseDrag(xPos, yPos);
			}
        }
		// Asignamos los datos de los sensores al evento touch
		touchEvent.getSensorsData().setAcelerometer(vec3(acelerometerX, acelerometerY, acelerometerZ));
		// Terminamos
        windowConnector->OnTouchEvent(&touchEvent);
    }
    else if (AINPUT_EVENT_TYPE_KEY == type) {
		// Conseguimos la tecla, mapeamos a ascii y se la pasamos a scaena
		int key_val = AKeyEvent_getKeyCode(event);
		int key_action = AKeyEvent_getAction(event);
		char asciiKey;
		if(getAsciiCharacterRepresentation(key_action, key_val, &asciiKey)){
			windowConnector->OnKeyDown(0, asciiKey);
		}
		__android_log_print(ANDROID_LOG_ERROR, "DEBUG", ("Key touchEvent received: " + StringUtils::toString(key_val)).c_str());
    }
    else {
		__android_log_print(ANDROID_LOG_ERROR, "DEBUG", "Unknown input touchEvent type");
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    static bool appResumed = false;
	struct engine* engine = (struct engine*)app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;

        case APP_CMD_INIT_WINDOW:
            // Iniciamos el time manager
			TimeManager::getInstance()->resume();

			// The window is being shown, get it ready.
            if (engine->app->window != NULL && !appResumed) {
                engine_init_display(engine);
                engine_draw_frame(engine);
				appResumed = true;
            }
			else{
				engine_resume_display(engine);
			}
            break;

        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed
			TimeManager::getInstance()->pause();
			engine_pause_display(engine);
            break;

        case APP_CMD_GAINED_FOCUS:
			// Prendemos otra vez el timeManager
			TimeManager::getInstance()->resume();

            // Empezamos a animar nuevamente
			engine->animating = 1;

			// When our app gains focus, we start monitoring the accelerometer.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                        engine->accelerometerSensor, (1000L/60)*1000);
            }
            break;

        case APP_CMD_LOST_FOCUS:
			// Pausamos el time manager
			TimeManager::getInstance()->pause();

            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
            }
            // Also stop animating.
            engine->animating = 0;
            break;
    }
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {

	saveAndroidStateOnNativeConnector(state);

    // Consigo el base path de la aplicacion y se lo seteo al window connector (el app config path es el mismo)
    ANativeActivity* nativeActivity = state->activity;
    const char* internalPath = nativeActivity->internalDataPath;
    WindowConnector::setBaseApplicationPath(string(internalPath) + "/");
    WindowConnector::setApplicationConfigurationPath(string(internalPath) + "/");

    // Hago una copia del dir assets al local del programa (esto solo es necesario hacerlo la
    // primera vez que se corre el programa o cuando hay cambios en assets).
    static bool firstTime = true;
    if(firstTime){   // SACAR ESTE IF Y VER COMO HACERLO SOLO UNA VEZ!!!!!!!!!!!!!!!!!!!
    	copyFilesFromAssetDirToProgramDir(state);
    	firstTime = false;
    }

	// Ejemplos de como llamar a metodos java. Estos deben definirse en la clase padre de esta en java
	// callVoidJavaMethod("nombreMetodoJava");
	// int result = callStringJavaMethod("nombreMetodoJava", "param1");
	// string stringResult = callStringJavaMethod("nombreMetodoJava", "param1");

    // Make sure glue isn't stripped.
    app_dummy();

    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    // Prepare to monitor accelerometer
    engine.sensorManager = ASensorManager_getInstance();
    engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
            ASENSOR_TYPE_ACCELEROMETER);
    engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
            state->looper, LOOPER_ID_USER, NULL, NULL);

    if (state->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        engine.state = *(struct saved_state*)state->savedState;
    }
	engine.animating = 1;

    // loop waiting for stuff to do.
    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(state, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) {
                if (engine.accelerometerSensor != NULL) {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                            &event, 1) > 0) {
						//__android_log_print(ANDROID_LOG_ERROR, "DEBUG", "accelerometer: x=%f y=%f z=%f", event.acceleration.x, event.acceleration.y, event.acceleration.z);
						acelerometerX = -event.acceleration.x / 10.0f;
						acelerometerY = -event.acceleration.y / 10.0f;
						acelerometerZ = -event.acceleration.z / 10.0f;

						// Actualizamos los datos de los sensores
						SensorsData sensorsData;
						sensorsData.setAcelerometer(vec3(acelerometerX, acelerometerY, acelerometerZ));
						windowConnector->OnSensorsUpdate(sensorsData);
                    }
                }
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                engine_term_display(&engine);
                return;
            }
        }

        if (engine.animating) {
            // Done with events; draw next animation frame.
            engine.state.angle += .01f;
            if (engine.state.angle > 1) {
                engine.state.angle = 0;
            }

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine_draw_frame(&engine);
        }
    }
}
//END_INCLUDE(all)
