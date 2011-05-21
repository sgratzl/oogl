get_filename_component(CURRENT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "current target ${CURRENT_NAME}")

resolve_files()
set(CURRENT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
group_files()

include_directories(${CURRENT_INCLUDE_DIRS})

find_package(DevIL REQUIRED)
find_binary_dirs(DevIL)
use_package(DevIL)

create_executable()

target_use_package(DevIL)

add_launcher()
