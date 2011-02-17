cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0011 NEW)
get_filename_component(PROJECT_NAME ${CMAKE_SOURCE_DIR} NAME)
message(STATUS ${PROJECT_NAME})
project(${PROJECT_NAME})

set(PROJECT_MAJOR_VERSION 0)
set(PROJECT_MINOR_VERSION 1)
SET(PROJECT_PATCH_VERSION 0)
set(PROJECT_BUILD_NUMBER 1)

set(PROJECT_VERSION_DESCRIPTION "${PROJECT_NAME} Binaries")
set(PROJECT_VERSION_PRODUCT_NAME "${PROJECT_NAME}")
set(PROJECT_VERSION_COPYRIGHT "Copyright (C) 2010")
set(PROJECT_VERSION_INTERNAL_NAME "${PROJECT_NAME}")

include(cmake/setup.cmake)

#include user settings
includeUserSettings()

add_subdirectory(external)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/internal/CMakeLists.txt")
	set(INTERNAL_AVALIABLE TRUE)
	set(INTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/internal")
	add_subdirectory(internal)
	include_directories(internal/snipplets)
endif()

add_subdirectory(exercises)

include(cmake/takedown.cmake)