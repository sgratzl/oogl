macro(init_target)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(CURRENT_OUTPUT_NAME ${CURRENT_NAME}${CMAKE_DEBUG_POSTFIX})
	else()
		set(CURRENT_OUTPUT_NAME ${CURRENT_NAME}${CMAKE_RELEASE_POSTFIX})
	endif()

	if(MSVC_IDE)
		#message(ERROR "setting prefix for msvc_ide")
		#set_target_properties(${CURRENT_NAME} PROPERTIES PREFIX ../)
		#set_target_properties(${CURRENT_NAME} PROPERTIES IMPORT_PREFIX ../)
	endif()
endmacro(init_target)

macro(set_default PROPERTY DEFAULT)
	if(NOT ${PROPERTY})
		set(${PROPERTY} ${DEFAULT})
	endif()
endmacro(set_default)

macro(pre_target)
	set_default(CURRENT_LABEL "${CURRENT_NAME}")
	set_default(CURRENT_DESCRIPTION "${PROJECT_VERSION_DESCRIPTION}")
	set_default(CURRENT_INTERNAL_NAME "${PROJECT_VERSION_INTERNAL_NAME} ${CURRENT_NAME}")

	if(IDE_TYPE STREQUAL "cdt")
		include_directories($ENV{INCLUDE}) #include the environment include to have it CDT
	endif()
endmacro(pre_target)

macro(create_library TYPE)
	message(STATUS "creating library of ${CURRENT_NAME}")

	pre_target()
	add_library(${CURRENT_NAME} ${TYPE} ${CURRENT_FILES})
	init_target()
endmacro(create_library)

macro(create_executable)
	message(STATUS "creating executable of ${CURRENT_NAME}")

	pre_target()
	add_executable(${CURRENT_NAME} ${CURRENT_FILES})
	init_target()
	set_target_properties(${CURRENT_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
	set_target_properties(${CURRENT_NAME} PROPERTIES RELEASE_POSTFIX "${CMAKE_RELEASE_POSTFIX}")
	set_target_properties(${CURRENT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "${CMAKE_RELWITHDEBINFO_POSTFIX}")
	set_target_properties(${CURRENT_NAME} PROPERTIES MINSIZEREL_POSTFIX "${CMAKE_MINSIZEREL_POSTFIX}")
	#set_target_properties(${CURRENT_NAME} PROPERTIES DEBUG_OUTPUT_NAME "${CURRENT_NAME}${CMAKE_DEBUG_POSTFIX}")
	#set_target_properties(${CURRENT_NAME} PROPERTIES RELEASE_OUTPUT_NAME "${CURRENT_NAME}${CMAKE_RELEASE_POSTFIX}")
	#set_target_properties(${CURRENT_NAME} PROPERTIES RELWITHDEBINFO_OUTPUT_NAME "${CURRENT_NAME}${CMAKE_RELWITHDEBINFO_POSTFIX}")
	#set_target_properties(${CURRENT_NAME} PROPERTIES MINSIZEREL_OUTPUT_NAME "${CURRENT_NAME}${CMAKE_MINSIZEREL_POSTFIX}")
endmacro(create_executable)

macro(create_static_library)
	create_library(STATIC)
endmacro(create_static_library)

macro(create_shared_library)
	create_library(SHARED)
endmacro(create_shared_library)


