find_path( lib3ds_INCLUDE_DIR NAMES lib3ds.h
	HINTS
		${LIB3DS_ROOT}
		${LIB3DS_ROOT}/include
		${LIB3DS_ROOT}/src
	)
find_library(lib3ds_LIBRARY NAMES lib3ds lib3ds-2_0 3ds 3ds-2_0
	HINTS
		${LIB3DS_ROOT}/Release
		${LIB3DS_ROOT}/lib
		${CMAKE_BINARY_DIR}/lib
	)
find_library(lib3ds_LIBRARY_DEBUG NAMES lib3dsd lib3ds-2_0d 3dsd 3ds-2_0d
	HINTS
		${LIB3DS_ROOT}/Debug
		${LIB3DS_ROOT}/lib
		${CMAKE_BINARY_DIR}/lib
	)

set( lib3ds_FOUND "NO" )
if(lib3ds_INCLUDE_DIR AND (lib3ds_LIBRARY OR lib3ds_LIBRARY_DEBUG))
	if(lib3ds_LIBRARY AND lib3ds_lib3ds_LIBRARY_DEBUG)
		set(lib3ds_LIBRARIES debug ${lib3ds_LIBRARY_DEBUG} optimized ${lib3ds_LIBRARY})
	elseif(lib3ds_LIBRARY)
		set(lib3ds_LIBRARIES ${lib3ds_LIBRARY})
	else()
		set(lib3ds_LIBRARIES ${lib3ds_LIBRARY_DEBUG})
	endif()
	SET(lib3ds_INCLUDE_DIRS ${lib3ds_INCLUDE_DIR})

	get_filename_component(lib3ds_LIBRARY_DIRS "${lib3ds_lib3ds_LIBRARY}" PATH)
	set( lib3ds_FOUND "YES" )
endif()

MARK_AS_ADVANCED(
	lib3ds_LIBRARIES
	lib3ds_LIBRARY_DIRS
	lib3ds_INCLUDE_DIR
	lib3ds_INCLUDE_DIRS
	lib3ds_LIBRARY
)
