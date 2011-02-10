function(add_launcher)
	message(STATUS "adding launcher for ${CURRENT_NAME}")

	if(CURRENT_BIN_DIRS)
		list(REMOVE_DUPLICATES CURRENT_BIN_DIRS)
	else()
		set(CURRENT_BIN_DIRS)
	endif()
	set(PROJECT_BIN_DIRS ${PROJECT_BIN_DIRS} ${CURRENT_BIN_DIRS} CACHE PATH "project binary dirs" FORCE)
	mark_as_advanced(PROJECT_BIN_DIRS)

	if(CURRENT_LIBRARY_DIRS)
		list(REMOVE_DUPLICATES CURRENT_LIBRARY_DIRS)
	else()
		set(CURRENT_LIBRARY_DIRS)
	endif()
	set(PROJECT_LIBRARY_DIRS ${PROJECT_LIBRARY_DIRS} ${CURRENT_LIBRARY_DIRS} CACHE PATH "project library dirs" FORCE)
	mark_as_advanced(PROJECT_LIBRARY_DIRS)

	if(CURRENT_INCLUDE_DIRS)
		list(REMOVE_DUPLICATES CURRENT_INCLUDE_DIRS)
	else()
		set(CURRENT_INCLUDE_DIRS)
	endif()
	set(PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} ${CURRENT_INCLUDE_DIRS} CACHE PATH "project include dirs" FORCE)
	mark_as_advanced(PROJECT_INCLUDE_DIRS)

	if(WIN32)
		configure_file(
			"${CMAKE_SOURCE_DIR}/cmake/start.${IDE_TYPE}.bat"
			"${CMAKE_BINARY_DIR}/start${CURRENT_NAME}.bat")
	elseif(UNIX)
		string(REPLACE ";" ":" CURRENT_FIXED_BIN_DIRS "${CURRENT_BIN_DIRS}")
		string(REPLACE ";" ":" CURRENT_FIXED_LIBRARY_DIRS "${CURRENT_LIBRARY_DIRS}")
		configure_file(
			"${CMAKE_SOURCE_DIR}/cmake/start.make.sh"
			"${CMAKE_BINARY_DIR}/start${CURRENT_NAME}.sh" IMMEDIATE)
		exec_program(chmod ARGS 755 "${CMAKE_BINARY_DIR}/start${CURRENT_NAME}.sh")
	endif()
	
	if (MSVC) #generate a .user file to set the working directory
		if (MSVC_VERSION EQUAL 1600)
			configure_file(${CMAKE_SOURCE_DIR}/cmake/vs2010.vcxproj.user.in
				${CMAKE_CURRENT_BINARY_DIR}/${CURRENT_NAME}.vcxproj.user @ONLY)
		else (MSVC_VERSION EQUAL 1500)
			configure_file(${CMAKE_SOURCE_DIR}/cmake/vs2008.vcproj.user.in
				${CMAKE_CURRENT_BINARY_DIR}/${CURRENT_NAME}.vcproj.user @ONLY)
		endif()
	endif()
endfunction(add_launcher)

function(add_project_launcher)
	if(WIN32 AND MSVC_IDE)
		if(DEFINED PROJECT_BIN_DIRS)
			list(REMOVE_DUPLICATES PROJECT_BIN_DIRS)
		endif()
		configure_file("${CMAKE_SOURCE_DIR}/cmake/open.vs.bat" "open${PROJECT_NAME}.bat")
	endif()
endfunction(add_project_launcher)
