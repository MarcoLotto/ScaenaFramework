# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_Include_Path=-I"..//Graphics/glew/include" -I"..//Graphics/WindowConnectors/freeglut/include" -I"..//Graphics/WindowConnectors/CommonInclude" -I"..//Graphics/WindowConnectors/glutConnector/include" -I"..//Graphics/include" -I"../glm-0.9.3.2" -I"../../../../../Program Files/Autodesk/FBX/FBX SDK/2013.3/include" -I"include"
Release_Include_Path=-I"..//Graphics/glew/include" -I"..//Graphics/WindowConnectors/freeglut/include" -I"..//Graphics/WindowConnectors/CommonInclude" -I"..//Graphics/WindowConnectors/glutConnector/include" -I"..//Graphics/include" -I"../glm-0.9.3.2" -I"../../../../../Program Files/Autodesk/FBX/FBX SDK/2013.3/include" -I"include"

# Library paths...
Debug_Library_Path=-L"..//Graphics/Debug" -L"..//Graphics/WindowConnectors/Debug" -L"../../../../../Program Files/Autodesk/FBX/FBX SDK/2013.3/lib/vs2010/gccx86" -L"/usr/lib/gcc4/x64"
Release_Library_Path=-L"..//Graphics/Release" -L"..//Graphics/WindowConnectors/Release" -L"../../../../../Program Files/Autodesk/FBX/FBX SDK/2013.3/lib/vs2010/gccx86" -L"/usr/lib/gcc4/x64"

# Additional libraries...
Debug_Libraries=-Wl,--start-group -lGraphics -lWindowConnector -lfbxsdk-2013.3-static -lGLEW -lGLU -lGL -lX11 -lpthread -lm -lrt -ldl -Wl,--end-group
Release_Libraries=-Wl,--start-group -lGraphics -lWindowConnector -lfbxsdk-2013.3-static -lGLEW -lGLU -lGL -lX11 -lpthread -lm -lrt -ldl -Wl,--end-group

# Preprocessor definitions...
Debug_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _CONSOLE 
Release_Preprocessor_Definitions=-D GCC_BUILD -D NDEBUG -D _CONSOLE 

# Implictly linked object files...
Debug_Implicitly_Linked_Objects=
Release_Implicitly_Linked_Objects=

# Compiler flags...
Debug_Compiler_Flags=-O0 -g -std=c++0x -DFBXSDK_NEW_API 
Release_Compiler_Flags=-O2 -std=c++0x -DFBXSDK_NEW_API 

# ******************Defino los directorios de sources para compilacion (para debug y release)********************************************

FILES_SRC_DIR := src/
FILES_MAIN_DIR := ./

CPP_FILES = $(wildcard $(FILES_SRC_DIR)/*.cpp) $(wildcard $(FILES_MAIN_DIR)/*.cpp)
C_FILES = 

# ******************Defino los directorios de salida de los obj para compilacion (para debug)********************************************

DEBUG_OBJ_OUTPUT := Debug/obj/

DEBUG_OBJ_FILES_CPP = $(addprefix $(DEBUG_OBJ_OUTPUT),$(CPP_FILES:.cpp=.o))
DEBUG_OBJ_FILES_C = $(addprefix $(DEBUG_OBJ_OUTPUT),$(C_FILES:.c=.o))
DEBUG_OBJ_FILES = $(DEBUG_OBJ_FILES_CPP) $(DEBUG_OBJ_FILES_C)

DEBUG_D_FILES_CPP = $(addprefix $(DEBUG_OBJ_OUTPUT),$(CPP_FILES:.cpp=.d))
DEBUG_D_FILES_C = $(addprefix $(DEBUG_OBJ_OUTPUT),$(C_FILES:.c=.d))
DEBUG_D_FILES = $(DEBUG_D_FILES_CPP) $(DEBUG_D_FILES_C)

# Builds all configurations for this project...
.PHONY: All
All: create_folders Debug Release 

# Builds the Debug configuration...
.PHONY: Debug
Debug: $(DEBUG_OBJ_FILES) $(DEBUG_D_FILES)
	$(CPP_COMPILER) $(DEBUG_OBJ_FILES) $(Debug_Library_Path) $(Debug_Libraries) -Wl,-rpath,./ -o Debug/ModelTransformer.exe
	
# ******************Defino las reglas de compilado para cada dependencia en debug***************************************
# Carpeta raiz
$(DEBUG_OBJ_OUTPUT)%.o: $(FILES_MAIN_DIR)%.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c $< $(Debug_Include_Path) -o $@	
$(DEBUG_OBJ_OUTPUT)%.d: $(FILES_MAIN_DIR)%.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM $< $(Debug_Include_Path) > $@
	
# Carpeta src
$(DEBUG_OBJ_OUTPUT)$(FILES_SRC_DIR)%.o: $(FILES_SRC_DIR)%.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c $< $(Debug_Include_Path) -o $@	
$(DEBUG_OBJ_OUTPUT)$(FILES_SRC_DIR)%.d: $(FILES_SRC_DIR)%.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM $< $(Debug_Include_Path) > $@

# *******************************************************************************************************************


# ******************Defino los directorios de salida de los obj para compilacion (para release)********************************************

# Los archivos a compilar (con las rutas de donde se toman y se dejan en Release y Release/obj)
RELEASE_OBJ_OUTPUT := Release/obj/

RELEASE_OBJ_FILES_CPP = $(addprefix $(RELEASE_OBJ_OUTPUT),$(CPP_FILES:.cpp=.o))
RELEASE_OBJ_FILES_C = $(addprefix $(RELEASE_OBJ_OUTPUT),$(C_FILES:.c=.o))
RELEASE_OBJ_FILES = $(RELEASE_OBJ_FILES_CPP) $(RELEASE_OBJ_FILES_C)

# Builds the Debug configuration...
.PHONY: Release
Release: $(RELEASE_OBJ_FILES)
	$(CPP_COMPILER) $< $(Release_Library_Path) $(Release_Libraries) -Wl,-rpath,./ -o Release/ModelTransformer.exe
	
# ******************Defino las reglas de compilado para cada dependencia en release***************************************
# Carpeta raiz
$(RELEASE_OBJ_OUTPUT)%.o: $(FILES_MAIN_DIR)%.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c $< $(Release_Include_Path) -o $@	

# Carpeta src
$(RELEASE_OBJ_OUTPUT)$(FILES_SRC_DIR)%.o: $(FILES_SRC_DIR)%.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c $< $(Release_Include_Path) -o $@	

# *******************************************************************************************************************

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p Debug/obj/src
	mkdir -p Release/obj/src

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f Debug/obj/*.o
	rm -f Debug/obj/*.d
	rm -f Debug/obj/src/*.o
	rm -f Debug/obj/src/*.d
	rm -f Debug/*.a
	rm -f Debug/*.so
	rm -f Debug/*.exe
	rm -f Release/obj/*.o
	rm -f Release/obj/*.d
	rm -f Release/obj/src/*.o
	rm -f Release/obj/src/*.d
	rm -f Release/*.a
	rm -f Release/*.so
	rm -f Release/*.exe



