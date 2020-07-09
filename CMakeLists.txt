cmake_minimum_required(VERSION 3.5)

project(JetsonDialog LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS WebEngineWidgets REQUIRED)
find_package(Qt5 COMPONENTS MultimediaWidgets REQUIRED)
find_package(Qt5 COMPONENTS Multimedia REQUIRED)

if(ANDROID)
  add_library(JetsonDialog SHARED
    main.cpp
    MainWindow.cpp
    MainWindow.hpp
	WebPage.cpp
	WebPage.hpp
	WebView.cpp
	WebView.hpp
	MainWindow.ui
	res.qrc
  )
else()
  add_executable(JetsonDialog
    main.cpp
	WebPage.cpp
	WebPage.hpp
	WebView.cpp
	WebView.hpp
    MainWindow.cpp
    MainWindow.hpp
    MainWindow.ui
	res.qrc
  )
endif()

target_link_libraries(JetsonDialog PRIVATE Qt5::Widgets Qt5::Network  Qt5::WebEngineWidgets Qt5::MultimediaWidgets Qt5::Multimedia)
