# - try to find glut library and include files
#  GLUT_INCLUDE_DIR, where to find GL/glut.h, etc.
#  GLUT_LIBRARIES, the libraries to link against
#  GLUT_FOUND, If false, do not try to use GLUT.
#  FREEGLUT_FOUND
# Also defined, but not for general use are:
#  GLUT_glut_LIBRARY = the full path to the glut library.
#  GLUT_Xmu_LIBRARY  = the full path to the Xmu library.
#  GLUT_Xi_LIBRARY   = the full path to the Xi Library.

#=============================================================================
# Copyright 2001-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distributed this file outside of CMake, substitute the full
#  License text for the above reference.)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
   # 64-bit project
   set(GLUT_BUILD_TYPE_ARCH 64)
else()
   # 32-bit project
   set(GLUT_BUILD_TYPE_ARCH 32)
endif()

IF (WIN32)

  FIND_PATH( GLUT_INCLUDE_DIR NAMES GL/glut.h
    HINTS
		${GLUT_ROOT}
		${GLUT_ROOT}/include )
  FIND_LIBRARY( GLUT_glut_LIBRARY NAMES freeglut glut glut32 
    HINTS
		${OPENGL_LIBRARY_DIR}
		${GLUT_ROOT}/Release
		${GLUT_ROOT}/lib${GLUT_BUILD_TYPE_ARCH}
		${GLUT_ROOT}/lib
    )
  FIND_LIBRARY( GLUT_glut_LIBRARY_DEBUG NAMES freeglutD glutD glut32D freeglut glut glut32
    HINTS
		${OPENGL_LIBRARY_DIR}
		${GLUT_ROOT}/Debug
		${GLUT_ROOT}/lib${GLUT_BUILD_TYPE_ARCH}
		${GLUT_ROOT}/lib
    )
ELSE (WIN32)

  IF (APPLE)
    # These values for Apple could probably do with improvement.
    FIND_PATH( GLUT_INCLUDE_DIR glut.h
      /System/Library/Frameworks/GLUT.framework/Versions/A/Headers
      ${OPENGL_LIBRARY_DIR}
      )
    SET(GLUT_glut_LIBRARY "-framework GLUT" CACHE STRING "GLUT library for OSX")
    SET(GLUT_cocoa_LIBRARY "-framework Cocoa" CACHE STRING "Cocoa framework for OSX")
  ELSE (APPLE)

    FIND_PATH( GLUT_INCLUDE_DIR GL/glut.h
      /usr/include/GL
      /usr/openwin/share/include
      /usr/openwin/include
      /opt/graphics/OpenGL/include
      /opt/graphics/OpenGL/contrib/libglut
      )

    FIND_LIBRARY( GLUT_glut_LIBRARY glut
      /usr/openwin/lib
      )

    FIND_LIBRARY( GLUT_Xi_LIBRARY Xi
      /usr/openwin/lib
      )

    FIND_LIBRARY( GLUT_Xmu_LIBRARY Xmu
      /usr/openwin/lib
      )

  ENDIF (APPLE)

ENDIF (WIN32)
SET( GLUT_FOUND "NO" )
IF(GLUT_INCLUDE_DIR AND GLUT_glut_LIBRARY)
	# Is -lXi and -lXmu required on all platforms that have it?
    # If not, we need some way to figure out what platform we are on.
    if(GLUT_glut_LIBRARY_DEBUG)
    	set(GLUT_LIBRARIES debug ${GLUT_glut_LIBRARY_DEBUG} optimized)
    endif()
    SET( GLUT_LIBRARIES
      ${GLUT_LIBRARIES}
      ${GLUT_glut_LIBRARY}
      ${GLUT_Xmu_LIBRARY}
      ${GLUT_Xi_LIBRARY}
      ${GLUT_cocoa_LIBRARY}
      )
	SET(GLUT_INCLUDE_DIRS ${GLUT_INCLUDE_DIR} ${GLUT_INCLUDE_DIR}/GL)

	GET_FILENAME_COMPONENT(GLUT_LIBRARY_DIRS "${GLUT_glut_LIBRARY}" PATH)
    SET( GLUT_FOUND "YES" )
ENDIF(GLUT_INCLUDE_DIR AND GLUT_glut_LIBRARY)

MARK_AS_ADVANCED(
  GLUT_LIBRARIES
  GLUT_LIBRARY_DIRS
  GLUT_INCLUDE_DIR
  GLUT_INCLUDE_DIRS
  GLUT_glut_LIBRARY
  GLUT_Xmu_LIBRARY
  GLUT_Xi_LIBRARY
  )
