get_filename_component(CURRENT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "current target ${CURRENT_NAME}")

resolve_files()
set(CURRENT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
group_files()

include_directories(${CURRENT_INCLUDE_DIRS})

find_package(OpenGL)
use_package(OpenGL)
find_binary_dirs(OpenGL)

find_package(GLM)
use_package(GLM)

find_internal_package(GLee)
use_package(GLee)

create_static_library()

target_use_package(OpenGL)
target_use_package(GLM)
target_use_package(GLee)

publish_package()
