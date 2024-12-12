cmake_minimum_required(VERSION 3.5)
project(window LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON) 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DSOURCE_DIR="${CMAKE_SOURCE_DIR}")
file(GLOB libs "./lib/*" "./Engine/lib/*")  
  
file(COPY ${libs} DESTINATION ${CMAKE_BINARY_DIR}/Release)
find_package(Qt6 COMPONENTS Widgets REQUIRED) 
include_directories(./include/Ela Engine/include Engine)
aux_source_directory(./src srcs)
add_subdirectory(Engine)  
# Specify MSVC UTF-8 encoding 
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
   
add_executable(${PROJECT_NAME}
    WIN32 # If you need a terminal for debug, please comment this statement 
    ${srcs}
    res.qrc
    ./include/Ela/ElaWidgetTools.qrc
) 
target_link_libraries( 
    ${PROJECT_NAME} PRIVATE Qt6::Widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/elawidgettools.lib
    EngineLib
)