cmake_minimum_required(VERSION 3.12)
project(Snake)

set(CMAKE_CXX_STANDARD 17)

add_executable(Snake main.cpp Snake.cpp Snake.h)

# Detect and add SFML
set(SOURCE_FILES main.cpp)
set(SFML_ROOT "C:\\Users\\tysse\\Documents\\SFML\\SFML-2.4.2-windows-gcc-6.1.0-mingw-64-bit\\SFML-2.4.2")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()