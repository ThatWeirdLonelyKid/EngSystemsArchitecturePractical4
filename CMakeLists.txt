cmake_minimum_required(VERSION 3.14)
project(OzAgriMate LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/third_party ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)
if(NOT SOURCES)
  message(FATAL_ERROR "No .cpp files found in ${CMAKE_SOURCE_DIR}/src - move src/ next to CMakeLists.txt")
endif()

add_executable(ozagrimate ${SOURCES})

find_package(Threads REQUIRED)
target_link_libraries(ozagrimate PRIVATE Threads::Threads)
if(WIN32)
  target_compile_definitions(ozagrimate PRIVATE _WIN32_WINNT=0x0A00)
  target_link_libraries(ozagrimate PRIVATE ws2_32)
endif()