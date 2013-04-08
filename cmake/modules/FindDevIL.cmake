# This module locates the developer's image library.
# http://openil.sourceforge.net/
#
# This module sets:
# IL_LIBRARIES the name of the IL library. These include the full path to the core DevIL library. This one has to be linked into the application.
# ILU_LIBRARIES the name of the ILU library. Again, the full path. This library is for filters and effects, not actual loading. It doesn't have to be linked if the functionality it provides is not used.
# ILUT_LIBRARIES the name of the ILUT library. Full path. This part of the library interfaces with OpenGL. It is not strictly needed in applications.
# IL_INCLUDE_DIR where to find the il.h, ilu.h and ilut.h files.
# IL_FOUND this is set to TRUE if all the above variables were set. This will be set to false if ILU or ILUT are not found, even if they are not needed. In most systems, if one library is found all the others are as well. That's the way the DevIL developers release it.

#=============================================================================
# Copyright 2008-2009 Kitware, Inc.
# Copyright 2008 Christopher Harvey
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

# TODO: Add version support.
# Tested under Linux and Windows (MSVC)

include(FindPackageHandleStandardArgs)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
   # 64-bit project
   set(IL_BUILD_TYPE_ARCH 64)
else()
   # 32-bit project
   set(IL_BUILD_TYPE_ARCH 32)
endif()

FIND_PATH(IL_INCLUDE_DIR IL/il.h
	PATH_SUFFIXES include${IL_BUILD_TYPE_ARCH} include
	DOC "The path the the directory that contains il.h"
	HINTS
		${DEVIL_ROOT}
		${DEVIL_ROOT}/include${IL_BUILD_TYPE_ARCH}
		${DEVIL_ROOT}/include
)

#MESSAGE("IL_INCLUDE_DIR is ${IL_INCLUDE_DIR}")

FIND_LIBRARY(IL_LIBRARIES
	NAMES IL DEVIL
	PATH_SUFFIXES lib${IL_BUILD_TYPE_ARCH} lib lib32
	DOC "The file that corresponds to the base il library."
	HINTS
		${DEVIL_ROOT}
)

#MESSAGE("IL_LIBRARIES is ${IL_LIBRARIES}")

FIND_LIBRARY(ILUT_LIBRARIES
	NAMES ILUT
	PATH_SUFFIXES lib${IL_BUILD_TYPE_ARCH} lib lib32
	DOC "The file that corresponds to the il (system?) utility library."
	HINTS
		${DEVIL_ROOT}
)

#MESSAGE("ILUT_LIBRARIES is ${ILUT_LIBRARIES}")

FIND_LIBRARY(ILU_LIBRARIES
	NAMES ILU
	PATH_SUFFIXES lib${IL_BUILD_TYPE_ARCH} lib lib32
	DOC "The file that corresponds to the il utility library."
	HINTS
		${DEVIL_ROOT}
)

#MESSAGE("ILU_LIBRARIES is ${ILU_LIBRARIES}")

FIND_PACKAGE_HANDLE_STANDARD_ARGS(IL DEFAULT_MSG
								IL_LIBRARIES ILU_LIBRARIES
								ILUT_LIBRARIES IL_INCLUDE_DIR)

if(IL_FOUND)
	set(DevIL_FOUND TRUE)
	set(DevIL_INCLUDE_DIR ${IL_INCLUDE_DIR})
	set(DevIL_INCLUDE_DIRS ${DevIL_INCLUDE_DIR} ${DevIL_INCLUDE_DIR}/IL)
	set(DevIL_LIBRARIES ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES})
	if(NOT WIN32)
		set(DevIL_DEFINITIONS -DILUT_USE_OPENGL)
	endif()
endif()

mark_as_advanced(
	DevIL_FOUND
	DevIL_INCLUDE_DIR
	DevIL_INCLUDE_DIRS
	DevIL_LIBRARIES
)

