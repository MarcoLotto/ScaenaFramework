# Application.mk

# In NDK r8e, we have to use GCC 4.7 for C++11 features like thread, mutex, etc.
#  The default GCC is 4.6, which does NOT define macro _GLIBCXX_HAS_GTHREADS,
#  so you’ll get errors like "mutex in namespace std does not name a type" etc.
NDK_TOOLCHAIN_VERSION := 4.8

# GNU STL implements most C++11 features. Use either gnustl_static or gnustl_shared
#  Without this your C++ code will not be able to access headers like <thread>, <mutex>
APP_STL := gnustl_static

APP_ABI := all
APP_CPPFLAGS := -O0 -std=gnu++11 -pthread -frtti -fexceptions -w
APP_PLATFORM := android-15
APP_OPTIM := debug
