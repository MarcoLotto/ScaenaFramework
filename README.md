# ScaenaFramework
A framework for developing cross-platform applications oriented to graphics rendering

Alpha state - Not ready for production yet

## Documentation
***Please see the project wiki for all the framework documentation:***

* [Go to Wiki](https://github.com/MarcoLotto/ScaenaFramework/wiki)

### Prerequisites
OpenGL 4.2 or superior must be installed on the client machine before executing the application on Windows and Linux. Make sure also that you have the lastest video drivers installed.
In Android and IOS the device must be compatible OpenGLES2 or OpenGLES3 (the mayor part of the market devices support this)

### Building on Ubuntu (or other distros)
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

If what you want is to build a particular project, then go to your project folder and execute:
```
make -f [projectName].makefile
``` 
### Building on Windows
You must have Visual Studio 2012 or superior installed in order to build the project. Then in order to compile the Graphics library, you simply open the Graphics.sln located in the Graphics folder and build it from Visual Studio. If you want to build an entire project as ExampleProject, go to the ExampleProject folder, open ExampleProject.sln and build it. The same applies to the ModelConverter module.  

### Building for Android
Android app can be built in Linux or Windows, but you must have GNU Make installed.
Also, you must have the Android SDK and NDK in your machine. This can be downloaded here:
* ***SDK (Android Studio is not required, just SDK):*** https://developer.android.com/studio/index.html
* ***NDK:*** https://developer.android.com/ndk/downloads/index.html

Once downloaded, go to the SDK folder and locate the ***SDK Manager***. Execute it and install the ***Android 4.0.3 (API 15)*** package (all the package).

Then go to YOUR project folder (ex. ExampleProject) where the file android-build.makefile resides and execute:
```
make -f android-build.makefile ANDROID_SDK_DIR=[path to sdk/tools] ANDROID_NDK_DIR=[path to ndk folder]
```
for example:
```
make -f android-build.makefile ANDROID_SDK_DIR=C:/Users/Marco/Documents/android-sdk/tools ANDROID_NDK_DIR=C:/Users/Marco/Documents/android-ndk-r10
```
This will generate the .apk and will install it on your device if connected.

### IOS
Comming soon...
