function(add_subdirectories)
	if(${ARGC} EQUAL 0)
		file(GLOB sub_dirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
	elseif(${ARGV0} STREQUAL "EXCLUDES")
		file(GLOB sub_dirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
		list(REMOVE_ITEM sub_dirs ${ARGV}) #TODO bug, if a directory called EXCLUDE exists
	else()
		set(sub_dirs ${ARGV})
	endif()
	foreach(sub_dir ${sub_dirs})
		if((IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${sub_dir}) AND
			(NOT(${elem} MATCHES "[._].*")) AND
			EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${sub_dir}/CMakeLists.txt)
			message(STATUS "adding directory ${sub_dir}")
			add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${sub_dir})
		endif()
	endforeach()
endfunction(add_subdirectories)
