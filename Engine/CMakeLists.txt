cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets OpenGL)
find_package(glm CONFIG REQUIRED)
   
file(GLOB_RECURSE Engine *.cpp *.h *.hpp)
add_library(EngineLib ${Engine})
if(CMAKE_BUILD_TYPE MATCHES Debug)  
    set(ASSIMP_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp-vc143-mtd.lib)
else()
    set(ASSIMP_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp-vc143-mt.lib)
endif()   
include_directories(./include ./include/bullet)
target_link_libraries(
    EngineLib        
    Qt6::Widgets      
    Qt6::OpenGLWidgets
    ${ASSIMP_LIB}
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/BulletCollision.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/BulletDynamics.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/LinearMath.lib
    glm::glm
)
                

