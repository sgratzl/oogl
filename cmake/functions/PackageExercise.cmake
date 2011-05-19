function(package_exercise_custom NAME TARGET)
	if(NOT INTERNAL_AVALIABLE)
		return()
	endif()

	find_package(ANT)

	if(ANT_FOUND)
		add_custom_target(${NAME}
			"${ANT_EXE}" "-buildfile" "assembly.xml" "${TARGET}"
			WORKING_DIRECTORY "${INTERNAL_DIR}"
			COMMENT "running ANT" VERBATIM)
	else()
		message(WARNING "can't find ANT check your settings otherwise you won't be able to build a package")
	endif()
endfunction(package_exercise_custom)

function(package_exercise)
	message(STATUS "adding package target for ${CURRENT_NAME}")
	get_filename_component(CURRENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
	package_exercise_custom(package${CURRENT_NAME} ${CURRENT_DIR})
endfunction(package_exercise)