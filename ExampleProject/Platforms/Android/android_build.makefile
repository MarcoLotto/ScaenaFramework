# Builds all configurations for this project...
.PHONY: Debug
Debug:
	# COMENZANDO LA COMPILACION
	../../../../ndk-build NDK_BUILD=1 APP_OPTIM=debug
	# COMPILACION FINALIZADA
	
.PHONY: Release
Release:
	# COMENZANDO LA COMPILACION
	../../../../ndk-build
	# COMPILACION FINALIZADA

