#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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
/*
* @brief inits the actual Vulkan functionallity
* @returns EXIT_SUCCESS or EXIT_FAILURE
*/
void vulkan_main_loop();
/*
* @brief inits the actual Vulkan functionallity
*/
int vulkan_pick_physical_device();

int vulkan_cleanup();
/*
* @brief Dumps all available vulkan extensions into a log file
*/
void vulkan_dump_extention_info();