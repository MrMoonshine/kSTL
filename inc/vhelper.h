#pragma once
//#define VK_USE_PLATFORM_WAYLAND_KHR
#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_X11
//#define GLFW_EXPOSE_NATIVE_WAYLAND
#include <GLFW/glfw3native.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
extern int errno;

#define VULKAN_EXTENSION_INFO_LOG_FILE "../vkext.log"
#define ERR_MSG_MALLOC "Failed to allocate Memory"



/*
* @brief wrapper function for all commonly used vulkan functions
* @returns EXIT_SUCCESS or EXIT_FAILURE
*/
int vulkan_run();
/*
* @brief inits the actual Vulkan functionallity
* @returns EXIT_SUCCESS or EXIT_FAILURE
*/
int vulkan_init();

int vulkan_create_window_surface();
/*
* @brief inits the actual Vulkan functionallity
* @returns EXIT_SUCCESS or EXIT_FAILURE
*/
void vulkan_main_loop();
/*
* @brief picks the optimal GPU to use Vulkan
* @returns EXIT_FAILURE if no suitable GPU was found
*/
int vulkan_pick_physical_device();
/*
* @brief frees the memory
*/
int vulkan_cleanup();
/*
* @brief Dumps all available vulkan extensions into a log file
*/
void vulkan_dump_extention_info();