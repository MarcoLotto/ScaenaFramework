LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

DEFINES += -D GCC_BUILD -D _DEBUG -D _CONSOLE 

C_STD = -O0 -std=gnu++11 -pthread -frtti -fexceptions -w
CXX_STD = -O0 -std=gnu++11 -pthread -frtti -fexceptions -w

LOCAL_MODULE    := libScaena
LOCAL_STATIC_LIBRARIES := ScaenaGraphics
LOCAL_LDLIBS     := -llog -landroid -lGLESv2 -lEGL
LOCAL_STATIC_LIBRARIES := android_native_app_glue

LOCAL_CFLAGS    := $(INCLUDES) $(C_STD) -D OPENGLES3 -D OPENGLES2
LOCAL_CXXFLAGS  := $(INCLUDES) $(CXX_STD) -D OPENGLES3 -D OPENGLES2

LOCAL_ALLOW_UNDEFINED_SYMBOLS := true

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include $(LOCAL_PATH)/ScaenaSrc/include $(LOCAL_PATH)/WindowConnectorSrc/include $(LOCAL_PATH)/WindowConnectorSrc/CommonInclude  $(LOCAL_PATH)/ScaenaSrc/glew/include $(LOCAL_PATH)//include $(LOCAL_PATH)/glm-0.9.3.2 $(LOCAL_PATH)/ExternalLibs $(LOCAL_PATH)/ScaenaSrc/zlib $(LOCAL_PATH)/ScaenaSrc/libpng $(LOCAL_PATH)/WindowConnectorSrc

# ******************Defino los directorios de sources para compilacion (para debug y release)********************************************
FILES_SCAENA_SRC_DIR := ScaenaSrc/src
FILES_CONNECTOR_DIR := WindowConnectorSrc/source
FILES_ZLIB_DIR := ScaenaSrc/zlib
FILES_LIBPNG_DIR := ScaenaSrc/libpng

FILES_SRC_DIR := src
FILES_MAIN_DIR := 

LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/$(FILES_SRC_DIR)/*.cpp) $(wildcard $(LOCAL_PATH)$(FILES_MAIN_DIR)/*.cpp) $(wildcard $(LOCAL_PATH)/$(FILES_CONNECTOR_DIR)/*.cpp) $(wildcard $(LOCAL_PATH)/$(FILES_SCAENA_SRC_DIR)/*.cpp) $(wildcard $(LOCAL_PATH)/$(FILES_ZLIB_DIR)/*.c) $(wildcard $(LOCAL_PATH)/$(FILES_LIBPNG_DIR)/*.c)

LOCAL_PATH := .

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
