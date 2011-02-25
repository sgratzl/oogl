get_filename_component(CURRENT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "current target ${CURRENT_NAME}")

resolve_files()
set(CURRENT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
group_files()

include_directories(${CURRENT_INCLUDE_DIRS})

find_package(OpenGL)
use_package(OpenGL)

find_package(GLM REQUIRED)
use_package(GLM)

find_internal_package(GLee)
use_package(GLee)

find_internal_package(lib3ds REQUIRED)
use_package(lib3ds)

find_package(DevIL REQUIRED)
use_package(DevIL)
find_binary_dirs(DevIL)

create_static_library()

target_use_package(OpenGL)
target_use_package(GLM)
target_use_package(GLee)
target_use_package(lib3ds)
target_use_package(DevIL)

publish_package()
