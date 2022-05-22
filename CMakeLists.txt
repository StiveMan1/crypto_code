cmake_minimum_required(VERSION 3.1)
project(crypto_code C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(PROJECT_VERSION 1.0)
set(DEFAULT_BUILD_TYPE "Release")
FILE(GLOB SOURCE_FILES "src/*.cpp")

include(GNUInstallDirs)
add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)
set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1)
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
