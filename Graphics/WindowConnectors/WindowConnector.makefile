# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_Include_Path=-I"CommonInclude" -I"freeglut/include" -I"glutConnector/include" -I"../../glm-0.9.3.2"
Release_Include_Path=-I"CommonInclude" -I"freeglut/include" -I"glutConnector/include" -I"../../glm-0.9.3.2"

# Preprocessor definitions...
Debug_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _CONSOLE -D FREEGLUT_STATIC
Release_Preprocessor_Definitions=-D GCC_BUILD -D NDEBUG -D _CONSOLE -D FREEGLUT_STATIC

# Implictly linked object files...
Debug_Implicitly_Linked_Objects=
Release_Implicitly_Linked_Objects=

# Compiler flags...
Debug_Compiler_Flags=-O0 -std=c++0x
Release_Compiler_Flags=-O2 -std=c++0x

# ******************Defino los directorios de sources para compilacion (para debug y release)********************************************

FILES_SRC_DIR := glutConnector/source/
FILES_FREEGLUT_DIR := freeglut/source/

CPP_FILES = $(wildcard $(FILES_SRC_DIR)/*.cpp)
C_FILES = $(wildcard $(FILES_FREEGLUT_DIR)/*.c)

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
	ar -rv Debug/libWindowConnector.a $(DEBUG_OBJ_FILES)
	
# ******************Defino las reglas de compilado para cada dependencia en debug***************************************
# Carpeta src
$(DEBUG_OBJ_OUTPUT)$(FILES_SRC_DIR)%.o: $(FILES_SRC_DIR)%.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c $< $(Debug_Include_Path) -o $@	
$(DEBUG_OBJ_OUTPUT)$(FILES_SRC_DIR)%.d: $(FILES_SRC_DIR)%.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM $< $(Debug_Include_Path) > $@
	
# Carpeta freeglut
$(DEBUG_OBJ_OUTPUT)$(FILES_FREEGLUT_DIR)%.o: $(FILES_FREEGLUT_DIR)%.c
	$(C_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c $< $(Debug_Include_Path) -o $@	
$(DEBUG_OBJ_OUTPUT)$(FILES_FREEGLUT_DIR)%.d: $(FILES_FREEGLUT_DIR)%.c
	$(C_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM $< $(Debug_Include_Path) > $@

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
	ar -rv Release/libWindowConnector.a $(RELEASE_OBJ_FILES)
	
# ******************Defino las reglas de compilado para cada dependencia en release***************************************
# Carpeta src
$(RELEASE_OBJ_OUTPUT)$(FILES_SRC_DIR)%.o: $(FILES_SRC_DIR)%.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c $< $(Release_Include_Path) -o $@	
	
# Carpeta freeglut
$(RELEASE_OBJ_OUTPUT)$(FILES_FREEGLUT_DIR)%.o: $(FILES_FREEGLUT_DIR)%.c
	$(C_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c $< $(Release_Include_Path) -o $@	

# *******************************************************************************************************************

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p Debug/obj/glutConnector/source/
	mkdir -p Debug/obj/freeglut/source
	mkdir -p Release/obj/glutConnector/source/
	mkdir -p Release/obj/freeglut/source

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f src/*.o
	rm -f src/*.d
	rm -f Debug/obj/*.o
	rm -f Debug/obj/*.d
	rm -f Debug/obj/glutConnector/source/*.o
	rm -f Debug/obj/glutConnector/source//*.d
	rm -f Debug/obj/freeglut/source/*.o
	rm -f Debug/obj/freeglut/source/*.d
	rm -f Debug/*.a
	rm -f Debug/*.so
	rm -f Debug/*.exe
	rm -f Release/obj/*.o
	rm -f Release/obj/*.d
	rm -f Release/obj/glutConnector/source/*.o
	rm -f Release/obj/glutConnector/source/*.d
	rm -f Release/obj/freeglut/source/*.o
	rm -f Release/obj/freeglut/source/*.d
	rm -f Release/*.a
	rm -f Release/*.so
	rm -f Release/*.exe

