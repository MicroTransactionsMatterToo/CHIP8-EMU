cmake_minimum_required(VERSION 3.6)
project(CHIP8_EMU)

include(FindPkgConfig)

set(CMAKE_CXX_STANDARD 14)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image)
add_definitions(-W -Wall -Wextra -pedantic)
find_package(OpenGL)
find_package(GLUT)

set(SOURCE_FILES src/CPU.cpp src/CPU.h src/gui.cpp src/gui.h)
add_executable(CHIP8_EMU ${SOURCE_FILES})
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(CHIP8_EMU ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES}
        ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})