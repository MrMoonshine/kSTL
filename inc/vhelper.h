#pragma once
#include <SDL.h>
#include <SDL_vulkan.h>
#include <stdint.h>

#define VK_USE_PLATFORM_WAYLAND_KHR
#include <vulkan/vulkan.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <vresult.h>
extern int errno;

#define VULKAN_EXTENSION_INFO_LOG_FILE "../vkext.log"
#define ERR_MSG_MALLOC "Failed to allocate Memory"

struct QueueFamilyIndices{
    uint32_t graphicsFamily;
    uint32_t presentFamily;

    bool graphicsFamilyValid;
    bool presentFamilyValid;
};

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
* @brief creates the presentation and graphics queue
*/
int vulkan_create_presentation_queue(struct QueueFamilyIndices *indices);
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
* @brief Creates the swap chain
*/
int vulkan_create_swap_chain();
/*
* @brief frees the memory
*/
int vulkan_cleanup();
/*
* @brief Dumps all available vulkan extensions into a log file
*/
void vulkan_dump_extention_info();