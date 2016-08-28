# ScaenaFramework
A framework for developing cross-platform applications oriented to graphics rendering

Alpha state - Not ready for production yet

## Prerequisites
OpenGL 4.2 or superior must be installed on the client machine before executing the application on Windows and Linux. Make sure also that you have the lastest video drivers installed.
In Android and IOS the device must be compatible OpenGLES2 or OpenGLES3 (the mayor part of the market devices support this)

## Building on Ubuntu (or other distros)
First you need to need the GL libraries:
```
sudo apt-get install freeglut3 freeglut3-dev
sudo apt-get install libglew-dev
```
Now, go to the proyect main folder (where the Makefile is) and run:
```
make
```
This will build all the modules (WindowConnector, Graphics and the example, testProject). It will not build ModelTransformer cause it is not allways necesary and you need de fbx library in order to do so.

## Building on Windows
You must have Visual Studio 2012 or superior installed in order to build the project. Then in order to compile the Graphics library, you simply open the Graphics.sln located in the Graphics folder and build it from Visual Studio. If you want to build an entire project as ExampleProject, go to the ExampleProject folder, open ExampleProject.sln and build it. The same applies to the ModelConverter module.  

## Building on Android
You must have the Android SDK and NDK in your machine (More on this later). Then go to YOUR project folder (ex. TestProject) where the file android-build.makefile resides and execute:
```
make -f android-build.makefile
```
This will generate the .apk and will install it on your device if connected.

## IOS
Comming soon...
