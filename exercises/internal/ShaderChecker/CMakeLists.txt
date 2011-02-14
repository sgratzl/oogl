get_filename_component(CURRENT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "current target ${CURRENT_NAME}")

resolve_files()
set(CURRENT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
group_files()

include_directories(${CURRENT_INCLUDE_DIRS})

find_package(GLUT REQUIRED)
use_package(GLUT)
find_binary_dirs(GLUT)

find_internal_package(common)
use_package(common)

create_executable()

target_use_package(GLUT)
target_use_package(common)

add_launcher()
