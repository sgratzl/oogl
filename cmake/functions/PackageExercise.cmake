function(package_exercise)
	if(NOT INTERNAL_AVALIABLE)
		return()
	endif()

	find_package(ANT)

	if(ANT_FOUND)
		message(STATUS "adding package target for ${CURRENT_NAME}")
		get_filename_component(CURRENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
		add_custom_target(package${CURRENT_NAME}
			"${ANT_EXE}" "-buildfile" "assembly.xml" "${CURRENT_DIR}"
			WORKING_DIRECTORY "${INTERNAL_DIR}"
			COMMENT "running ANT" VERBATIM)
	else()
		message(WARNING "can't find ANT check your settings otherwise you won't be able to build a package")
	endif()
endfunction(package_exercise)

function(packageall_exercises)
	if(NOT INTERNAL_AVALIABLE)
		return()
	endif()

	find_package(ANT)

	if(ANT_FOUND)
		add_custom_target(packageAllExercises
			"${ANT_EXE}" "-buildfile" "assembly.xml" "complete"
			WORKING_DIRECTORY "${INTERNAL_DIR}"
			COMMENT "running ANT" VERBATIM)
	else()
		message(WARNING "can't find ANT check your settings otherwise you won't be able to build a package")
	endif()
endfunction(packageall_exercises)

function(packagehalf_exercises)
	if(NOT INTERNAL_AVALIABLE)
		return()
	endif()

	find_package(ANT)

	if(ANT_FOUND)
		add_custom_target(packageHalfTimeExercises
			"${ANT_EXE}" "-buildfile" "assembly.xml" "halftime"
			WORKING_DIRECTORY "${INTERNAL_DIR}"
			COMMENT "running ANT" VERBATIM)
	else()
		message(WARNING "can't find ANT check your settings otherwise you won't be able to build a package")
	endif()
endfunction(packagehalf_exercises)

