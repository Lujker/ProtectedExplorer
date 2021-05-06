cmake_minimum_required(VERSION 3.14)

project(ProtectedExplorer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package(...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS
    Core Quick Xml sql LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
    Core Quick Xml sql LinguistTools REQUIRED)

set(PROJECT_SOURCES
        ./src/main.cpp
        ./src/loger.cpp
        ./src/settingscontroller.cpp
        ./src/folderexpl.cpp
        ./qml/qml.qrc
)

set(HEADER_FILE_ONLY
        ./include/folderexpl.h
        ./include/loger.h
        ./include/settingscontroller.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ProtectedExplorer
        ${PROJECT_SOURCES}
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR})
else()
    if(ANDROID)
        add_library(ProtectedExplorer SHARED
            ${PROJECT_SOURCES} ${HEADER_FILE_ONLY}
        )
    else()
        add_executable(ProtectedExplorer
          ${PROJECT_SOURCES} ${HEADER_FILE_ONLY}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR})
endif()

target_include_directories(ProtectedExplorer PRIVATE ./include)

target_compile_definitions(ProtectedExplorer
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(ProtectedExplorer
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::Xml
        Qt${QT_VERSION_MAJOR}::Sql)
