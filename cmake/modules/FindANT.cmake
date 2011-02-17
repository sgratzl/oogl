find_program(ANT_EXE
	NAMES "ant"
	HINTS
		${ANT_HOME}/bin
		ENV{ANT_HOME}/bin
	DOC "Ant - build generator for Java"
)

if(ANT_EXE)
	set(ANT_FOUND TRUE)
	get_filename_component(ANT_BIN_DIR "${ANT_EXE}" PATH)
	if(NOT ANT_HOME)
		get_filename_component(ANT_HOME "${ANT_BIN_DIR}" PATH)
	endif()
endif()

mark_as_advanced(
	ANT_EXE
	ANT_HOME
	ANT_BIN_DIR
)