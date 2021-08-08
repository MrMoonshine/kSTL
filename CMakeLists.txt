
cmake_minimum_required(VERSION 3.10)

project(kSTL C)

add_definitions(
    -DAPP_TITLE="kSTL"
    -DAPP_HEIGHT=600
    -DAPP_WIDTH=800
    -DTEST_IMAGE="/home/david/Bilder/carBackground/67e77813c76ba4e924d2abcd952820fc.jpg"
)

file(GLOB SRC_FILES
    "src/*.c"
)

file(GLOB HDR_FILES
    "inc/*.h"
)

# some test warning levels for GCC
set(WARNING_LEVELS_GCC
    -Werror
    -Wno-deprecated
)
set(WARNING_LEVELS_GCC_DEBUG
    -Wno-deprecated
)  

#add_executable(${PROJECT_NAME} ${THIS_DIR})
set(CMAKE_WARN_DEPRECATED FALSE)
set(CMAKE_C_COMPILER gcc)

#find further libraries: dpkg -L libglfw3
#GTK
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)

find_package(GLEW REQUIRED)

if (NOT OpenGL_OpenGL_FOUND)
    message (FATAL_ERROR "OpenGL not found")
endif ()

#CGLM
add_library(cglm SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(cglm PROPERTIES
  IMPORTED_LOCATION "/usr/lib/x86_64-linux-gnu/libcglm.so"
  INTERFACE_INCLUDE_DIRECTORIES "/usr/include/cglm"
)
#open libm
add_library(openlibm SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(openlibm PROPERTIES
  IMPORTED_LOCATION "/usr/lib/x86_64-linux-gnu/libopenlibm.so"
  INTERFACE_INCLUDE_DIRECTORIES "/usr/include/openlibm"
)

include_directories(
    /usr/include/
	./inc/
)

add_executable (${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} openlibm)
target_link_libraries(${PROJECT_NAME} cglm)
target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})