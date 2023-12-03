cmake_minimum_required(VERSION 3.26)
project(splinart)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_EXAMPLES "Build the examples of splinart" OFF)
option(BUILD_TESTS "Build the tests of splinart" OFF)

find_package(xtensor)
find_package(CLI11)
set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_library(splinart src/shapes.cpp)
target_include_directories(
    splinart
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(splinart xtensor ${OpenCV_LIBS})

add_executable(splinart-cpp src/command_line.cpp)

if(MSVC)
    target_compile_options(splinart-cpp PUBLIC /MP /bigobj)
endif()

target_link_libraries(splinart-cpp splinart CLI11::CLI11 ${OpenCV_LIBS})

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()


include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS splinart splinart-cpp
        EXPORT splinart-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/splinart)
