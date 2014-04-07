
set(EXTERNAL_LIB_DIR "${CMAKE_SOURCE_DIR}/external")

set(GLM_ROOT "${EXTERNAL_LIB_DIR}/glm-0.9.5.2")

if(WIN32)
	set(GLUT_ROOT "${EXTERNAL_LIB_DIR}/freeglut-2.8.1")
	set(DEVIL_ROOT "${EXTERNAL_LIB_DIR}/DevIL-1.7.8")
elseif(APPLE)
	set(DEVIL_ROOT "/opt/local")
endif()