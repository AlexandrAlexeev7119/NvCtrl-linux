cmake_minimum_required(VERSION 3.5)

project(NvCtrl-Linux VERSION 1.4.33 LANGUAGES CXX)

if(NOT WIN32)
    string(ASCII 27 Esc)
    set(ColourReset "${Esc}[m")
    set(ColourBold  "${Esc}[1m")
    set(Red         "${Esc}[31m")
    set(Green       "${Esc}[32m")
    set(Yellow      "${Esc}[33m")
    set(Blue        "${Esc}[34m")
    set(Magenta     "${Esc}[35m")
    set(Cyan        "${Esc}[36m")
    set(White       "${Esc}[37m")
    set(BoldRed     "${Esc}[1;31m")
    set(BoldGreen   "${Esc}[1;32m")
    set(BoldYellow  "${Esc}[1;33m")
    set(BoldBlue    "${Esc}[1;34m")
    set(BoldMagenta "${Esc}[1;35m")
    set(BoldCyan    "${Esc}[1;36m")
    set(BoldWhite   "${Esc}[1;37m")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(USE_LTO ON)
set(USE_STATIC_LINKING ON) # not affected, but why no

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOGEN_VERBOSE OFF)
set(CMAKE_AUTOUIC_SEARCH_PATHS "./ui")
set(CMAKE_AUTOGEN_BUILD_DIR "./include")



find_package(Qt5 COMPONENTS Widgets DBus REQUIRED)

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE HDR include/*.hpp)
file(GLOB_RECURSE UI ui/*.ui)
file(GLOB_RECURSE RESOURCES res/*.qrc)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

if(USE_LTO)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
    message("Link time optimizations: ${Green}ON")
else()
    message("Link time optimizations: ${Red}OFF")
endif()

if(USE_STATIC_LINKING)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
    message("Static linking: ${Green}ON")
else()
    message("Static linking: ${Red}OFF")
endif()

if("${CMAKE_CXX_COMPILER}" MATCHES "mingw")
    set(BUILD_WITH_MINGW ON)
endif()

set(PROJECT_SOURCES
    ${SRC}
    ${HDR}
    ${UI}
    ${EXT_LIBS}
)

include_directories(${INCLUDE_DIRECTORIES}
    ./include
    ./lib/NVMLpp/include
    ./lib/spdlog/include
    ./lib/nlohmann-json/include
)

link_directories(
    ${LINK_DIRECTORIES}
    ${CMAKE_BINARY_DIR}
)

if(BUILD_WITH_MINGW)
    link_directories(
        ${LINK_DIRECTORIES}
        /usr/x86_64-w64-mingw32/bin/
        /usr/x86_64-w64-mingw32/lib/
    )
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector")
    message("Target platform: ${Green}Win32")
else()
    message("Target platform: ${Green}Linux")
endif()



add_subdirectory(lib/NVMLpp)
add_subdirectory(lib/spdlog)

add_compile_definitions(APP_VERSION="${PROJECT_VERSION}")
add_compile_definitions(APP_BUILD_TYPE="${CMAKE_BUILD_TYPE}")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::DBus NVMLpp spdlog)

write_file(../.last_version "${PROJECT_VERSION}")
