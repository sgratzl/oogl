#set module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules )

#set default build type
if(CMAKE_BUILD_TYPE)
	message(STATUS "have build type with ${CMAKE_BUILD_TYPE}")
else()
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "build type (Release/Debug)")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release)
	message(STATUS "set build type to ${CMAKE_BUILD_TYPE}")
endif()

#set default install prefix
if (NOT CMAKE_INSTALL_PREFIX_INITIALZED OR NOT CMAKE_INSTALL_PREFIX)
	set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH "Install path prefix, prepended onto install directories" FORCE)
	set(CMAKE_INSTALL_PREFIX_INITIALZED ON CACHE INTERNAL "flag whether install prefix already set")
endif()

#include defined funtions
file(GLOB functions RELATIVE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/cmake/functions/*.cmake)
foreach(function ${functions})
	message(STATUS "including function: ${function}")
	include(${function})
endforeach()

#include defined makros
file(GLOB makros RELATIVE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/cmake/makros/*.cmake)
foreach(makro ${makros})
	message(STATUS "including makro: ${makro}")
	include(${makro})
endforeach()


#set generators
if(WIN32 AND MSVC_IDE)
	set(IDE_TYPE vs)
elseif(DEFINED CMAKE_ECLIPSE_EXECUTABLE)
	set(IDE_TYPE cdt)
else()
	set(IDE_TYPE unknown)
endif()

#set output locations
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
if(MSVC_IDE)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
else()
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_DEBUG_POSTFIX d)

#init version properties
if(NOT PROJECT_MAJOR_VERSION)
	set(PROJECT_MAJOR_VERSION 0)
endif()
if(NOT PROJECT_MINOR_VERSION)
	set(PROJECT_MINOR_VERSION 1)
endif()
if(NOT PROJECT_PATCH_VERSION)
	set(PROJECT_PATCH_VERSION 0)
endif()
if(NOT PROJECT_BUILD_NUMBER)
	set(PROJECT_BUILD_NUMBER 0)
endif()

set(PROJECT_VERSION "${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_VERSION} ${PROJECT_BUILD_VERSION}" CACHE STRING "the project version")


if(IDE_TYPE STREQUAL "cdt")
	set(ECLIPSE_CDT4_GENERATE_SOURCE_PROJECT ON)
endif()
