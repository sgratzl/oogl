
find_path(GLM_INCLUDE_DIR
	NAMES glm/glm.hpp
	HINTS
		${GLM_ROOT}
		${GLM_ROOT}/include )

if(GLM_INCLUDE_DIR)
	set(GLM_FOUND "YES")
	set(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})
endif()

mark_as_advanced(
	GLM_INCLUDE_DIR
	GLM_INCLUDE_DIRS
)