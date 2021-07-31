#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
extern int errno;

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
int vulkan_main_loop();
int vulkan_cleanup();