cmake_minimum_required(VERSION 3.5)

project(gwepp VERSION 1.0.3 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS "./ui")
set(CMAKE_AUTOGEN_BUILD_DIR "./include")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOGEN_VERBOSE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
add_subdirectory(lib/NVMLpp/)

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE HDR include/*.hpp)
file(GLOB_RECURSE UI ui/*.ui)

set(PROJECT_SOURCES
    ${SRC}
    ${HDR}
    ${UI}
    ${EXT_LIBS}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
    )
endif()

include_directories(${INCLUDE_DIRECTORIES} ./include ./lib/NVMLpp/include)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets NVMLpp)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GWEpp)
endif()
