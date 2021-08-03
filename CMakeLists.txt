
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

#GLFW
add_library(glfw3 SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(glfw3 PROPERTIES
  IMPORTED_LOCATION "/usr/lib/x86_64-linux-gnu/libglfw.so.3"
  INTERFACE_INCLUDE_DIRECTORIES "/usr/include/GLFW"
)

#SDL2
find_package(SDL2 REQUIRED)
if (NOT SDL2_FOUND)
    message (FATAL_ERROR "SDL not found")
else()
    include_directories (${SDL2_INCLUDE_DIRS})
    link_directories (${SDL2_LIBRARIES})
endif ()

#Vulkan
find_package(Vulkan REQUIRED)
if (NOT Vulkan_FOUND)
    message (FATAL_ERROR "VULKAN not found")
else()
    include_directories (${Vulkan_INCLUDE_DIRS})
    link_directories (${Vulkan_LIBRARIES})
endif ()

include_directories(
    /usr/include/
	./inc/
)

add_executable (${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} glfw3)
target_link_libraries(${PROJECT_NAME} SDL2)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})