#VARIABLES DE CONFIGURACION DE ENTORNO*************************************************************************
ANDROID_SDK_DIR = C:/Users/Marco/Documents/android-sdk/tools
ANDROID_NDK_DIR = C:/Users/Marco/Documents/android-ndk-r10

ANDROID_PACKER_FILENAME = android.bat  # Si estoy compilando desde Windows (con android adb de windows)
## ANDROID_PACKER_FILENAME = android      # Si estoy compilando desde Linux (con android adb de linux)
#**************************************************************************************************************

# Ejecutar para empaquetar e instalar en android, por defecto en modo debug
.PHONY: Android_install
Android_install: Android Android_package
	ant debug install -buildfile ./build/build.xml

# Ejecutar para compilar en android, por defecto en modo debug
.PHONY: Android
Android: android_deploy android_debug_deploy

# Ejecutar para compilar en android en modo release
.PHONY: Android_Release
Android_Release: android_deploy android_release_deploy

# Genaracion de configuracion del proyecto android y empaquetado en .apk
.PHONY: Android_package
Android_package: 
	$(ANDROID_SDK_DIR)/$(ANDROID_PACKER_FILENAME) update project -p . -t android-15 --path ./build
	ant debug -buildfile ./build/build.xml

# Deploy del proyecto en la carpeta de build para android
.PHONY: android_deploy
android_deploy:
	# COMENZANDO EL DEPLOY
	# Copio todos los assets
	mkdir -p ./build/assets
	rsync -rupE ./Definitions ./build/assets
	rsync -rupE ./Modelos ./build/assets
	rsync -rupE ./Textures ./build/assets
	rsync -rupE ./Platforms/Android/Shaders ./build/assets	
	
	# Copio los archivos especificos de la plataforma
	mkdir -p ./build/jni
	rsync -rupE ./Platforms/Android/AndroidManifest.xml ./build/
	rsync -rupE ./Platforms/Android/android_build.makefile ./build/
	rsync -rupE ./Platforms/Android/Android.mk ./build/jni/
	rsync -rupE ./Platforms/Android/Application.mk ./build/jni/
	rsync -rupE ./Platforms/Android/src ./build
	
	# Copio los fuentes del proyecto
	mkdir -p ./build/jni
	rsync -rupE ./src ./build/jni
	rsync -rupE ./include ./build/jni
	
	# Copio los fuentes de Scaena
	mkdir -p ./build/jni/ScaenaSrc
	rsync -rupE ../Graphics/src ./build/jni/ScaenaSrc
	rsync -rupE ../Graphics/include ./build/jni/ScaenaSrc
	
	# Remuevo el archivo "GLSLProgramGLImplementation" porque no es necesario en android
	rm ./build/jni/ScaenaSrc/src/GLSLProgramGLImplementation.cpp
	
	# Remuevo el archivo "OpenglDevice" porque no es necesario en android
	rm ./build/jni/ScaenaSrc/src/OpenglDevice.cpp
	
	# Remuevo el archivo "OpenglConstantCatalog" porque no es necesario en android
	rm ./build/jni/ScaenaSrc/src/OpenglConstantCatalog.cpp
	
	# Copio los fuentes de zlib
	mkdir -p ./build/jni/ScaenaSrc/zlib
	rsync -rupE ../Graphics/zlib ./build/jni/ScaenaSrc
	
	# Copio los fuentes de libpng
	mkdir -p ./build/jni/ScaenaSrc/libpng
	rsync -rupE ../Graphics/libpng ./build/jni/ScaenaSrc
	
	# Copio los fuentes de GLES (2 y 3)
	mkdir -p ./build/jni/ScaenaSrc/glew/include/
	rsync -rupE ../Graphics/glew/include/GLES/GLES2 ./build/jni/ScaenaSrc/glew/include/
	rsync -rupE ../Graphics/glew/include/GLES/GLES3 ./build/jni/ScaenaSrc/glew/include/
	rsync -rupE ../Graphics/glew/include/GLES/KHR ./build/jni/ScaenaSrc/glew/include/
	
	# Copio los fuentes del conector de ventana
	mkdir -p ./build/jni/WindowConnectorSrc
	rsync -rupE ../Graphics/WindowConnectors/jniConnector/source ./build/jni/WindowConnectorSrc/
	rsync -rupE ../Graphics/WindowConnectors/jniConnector/include ./build/jni/WindowConnectorSrc/
	rsync -rupE ../Graphics/WindowConnectors/CommonInclude ./build/jni/WindowConnectorSrc/
	
	# Copio los fuentes de glm
	mkdir -p ./build/jni
	rsync -rupE ../glm-0.9.3.2 ./build/jni
	
	# Copio librerias externas de scaena
	rsync -rupE ../ExternalLibs ./build/jni
	
	# DEPLOY FINALIZADO

# Proceso de compilacion debug de android ndk
.PHONY: android_debug_deploy
android_debug_deploy:
	# COMENZANDA COMPILACION EN MODO DEBUG
	$(ANDROID_NDK_DIR)/ndk-build -C ./build NDK_DEBUG=1
	# FINALIZADA COMPILACION EN MODO DEBUG
	
# Proceso de compilacion release de android ndk
.PHONY: android_release_deploy
android_release_deploy:
	# COMENZANDA COMPILACION EN MODO RELEASE
	$(ANDROID_NDK_DIR)/ndk-build -C ./build NDK_DEBUG=0
	# FINALIZADA COMPILACION EN MODO RELEASE

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -r ./build



