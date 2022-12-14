cmake_minimum_required(VERSION 3.5)

project(NvCtrl-Linux VERSION 1.4.31 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS "./ui")
set(CMAKE_AUTOGEN_BUILD_DIR "./include")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOGEN_VERBOSE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets DBus REQUIRED)

add_subdirectory(lib/NVMLpp)
add_subdirectory(lib/spdlog)

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE HDR include/*.hpp)
file(GLOB_RECURSE UI ui/*.ui)
file(GLOB_RECURSE RESOURCES res/*.qrc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -flto -static-libstdc++ -static-libgcc")

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

add_compile_definitions(APP_VERSION="${PROJECT_VERSION}")
add_compile_definitions(APP_BUILD_TYPE="${CMAKE_BUILD_TYPE}")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::DBus NVMLpp spdlog)

write_file(../.last_version "${PROJECT_VERSION}")
