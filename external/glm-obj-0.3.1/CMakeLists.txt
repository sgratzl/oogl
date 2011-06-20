set(CURRENT_NAME glmobj)

resolve_files()
set(CURRENT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
group_files()

include_directories(${CURRENT_INCLUDE_DIRS})

add_definitions(-DHAVE_DEVIL)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(OpenGL)
use_package(OPENGL)

find_package(DevIL REQUIRED)
use_package(DevIL)

create_static_library()

set_target_properties(${CURRENT_NAME} PROPERTIES VERSION 0.3.1)

target_use_package(OPENGL)
target_use_package(DevIL)

publish_package()