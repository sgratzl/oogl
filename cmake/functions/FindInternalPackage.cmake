function(find_internal_package Libname)
	message(STATUS "looking for internal library ${Libname}")
	if(NOT ${Libname}_NAME)
		message(ERROR "can't find internal library ${Libname}")
	endif()
	#rest is already in the cache
endfunction(find_internal_package)