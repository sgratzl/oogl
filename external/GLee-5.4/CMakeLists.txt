set(CURRENT_NAME GLee)
message(STATUS "current target ${CURRENT_NAME}")

resolve_files()
set(CURRENT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(${CURRENT_INCLUDE_DIRS})

find_package(OpenGL)
use_package(OpenGL)

create_static_library()

target_use_package(OpenGL)

publish_package()
