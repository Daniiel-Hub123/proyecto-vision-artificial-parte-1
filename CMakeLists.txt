cmake_minimum_required(VERSION 3.16)
project(ProyectoVisionParte1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Basado en los CMakeLists.txt de los ejemplos de la asignatura.
# La ruta local de OpenCV se indica al configurar, no se fija en este archivo.
find_package(OpenCV REQUIRED COMPONENTS core highgui videoio)

add_executable(vision_parte1 src/Principal.cpp)
target_include_directories(vision_parte1 PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(vision_parte1 PRIVATE ${OpenCV_LIBS})

message(STATUS "OpenCV encontrado: ${OpenCV_VERSION}")
