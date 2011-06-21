macro(resolve_files)
	file(GLOB_RECURSE CURRENT_HEADERS *.h *.hpp)
	file(GLOB_RECURSE CURRENT_SOURCES *.cpp *.cxx *.c++)
	file(GLOB_RECURSE CURRENT_INLINES *.inl)

	set(CURRENT_FILES ${CURRENT_HEADERS} ${CURRENT_SOURCES} ${CURRENT_INLINES})
endmacro(resolve_files)