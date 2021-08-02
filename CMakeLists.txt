cmake_minimum_required(VERSION 3.10)

project(kSTL C)

add_definitions(
    -DAPP_TITLE="Vanilla Vulkan"
    -DAPP_HEIGHT=600
    -DAPP_WIDTH=800
)

file(GLOB SRC_FILES
    "src/*.c"
)

file(GLOB HDR_FILES
    "inc/*.h"
)

#add_executable(${PROJECT_NAME} ${THIS_DIR})

set(CMAKE_C_COMPILER gcc)

#find further libraries: dpkg -L libglfw3

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

#target_link_libraries(${PROJECT_NAME} glfw3)
target_link_libraries(${PROJECT_NAME} SDL2)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)