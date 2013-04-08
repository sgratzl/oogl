#set module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules )

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR "don't use the source directory as binary directory")
endif()

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

#specify 32 or 64 bit build
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
   # 64-bit project
   message(STATUS "building 64-bit project")
   set(PROJECT_BUILD_TYPE_32 OFF)
   set(PROJECT_BUILD_TYPE_64 ON)
   set(PROJECT_BUILD_TYPE_ARCH 64)
else()
   # 32-bit project
   message(STATUS "building 32-bit project")
   set(PROJECT_BUILD_TYPE_32 ON)
   set(PROJECT_BUILD_TYPE_64 OFF)
   set(PROJECT_BUILD_TYPE_ARCH 32)
endif()

#set generators
if(WIN32 AND MSVC_IDE)
	set(IDE_TYPE vs)
	if (PROJECT_BUILD_TYPE_64)
		set(IDE_BUILD_PLATFORM "x64")
	else()
		set(IDE_BUILD_PLATFORM "Win32")
	endif()
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

#if(CMAKE_COMPILER_IS_GNUCXX)
#    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++0x) #enable c++0x features
#endif()
