//#include <vhelper.h>
#include "../inc/vhelper.h"
#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

static SDL_Window* window;

static VkInstance vkinst;

static VkPhysicalDevice physical_device = VK_NULL_HANDLE;
static VkDevice logical_device = VK_NULL_HANDLE;

static VkSurfaceKHR surface;
static VkQueue presentation_queue, graphics_queue;

static const char *TAG = "Vulkan";

/*
* @brief inits the GLFW window
* @returns EXIT_SUCCESS or EXIT_FAILURE
*/
static int vulkan_init_window(){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    window = SDL_CreateWindow(
        APP_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        APP_WIDTH,
        APP_HEIGHT,
        SDL_WINDOW_VULKAN
    );

    if(!window){
        SDL_Quit();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

static bool vulkan_has_suitable_queue_family(VkPhysicalDevice device, struct QueueFamilyIndices* qfi){
    if(qfi)
    qfi->presentFamilyValid = false;

    bool suitable = false;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

    VkQueueFamilyProperties* queueFamilies;
    queueFamilies = (VkQueueFamilyProperties*)malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    for(int a = 0; a < queueFamilyCount; a++){
        if(queueFamilies[a].queueFlags & VK_QUEUE_GRAPHICS_BIT){
            suitable = true;
            if(qfi){
                qfi->graphicsFamily = a;

                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, a, surface, &presentSupport);
                if(presentSupport){
                    qfi->presentFamilyValid = true;
                    qfi->presentFamily = a;
                }
            }
            
        }
    }

    if(qfi){
        qfi->graphicsFamilyValid = suitable;
    }
    free(queueFamilies);
    return suitable;
}

static int vulkan_get_arbitrary_device_suitability(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    printf("[%s] Checking: %s\n",TAG, deviceProperties.deviceName);

    if(
        deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
        deviceFeatures.geometryShader
    ){
        int score = 0;

        // Discrete GPUs have a significant performance advantage
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        score += 1000;

        // Maximum possible size of textures affects graphics quality
        score += deviceProperties.limits.maxImageDimension2D;

        // Application can't function without geometry shaders
        if (!deviceFeatures.geometryShader) {
            return 0;
        }

        return score;
    }else{
        return 0;
    }
}

int vulkan_run(){
    vulkan_init_window();
    vulkan_init();
    
    vulkan_dump_extention_info();
    vulkan_main_loop();
    vulkan_cleanup();
    return EXIT_SUCCESS;
}

int vulkan_init(){
    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = APP_TITLE,
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0
    };    

    VkInstanceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t sdlExtensionCount = 0;
    SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, NULL);

    const char** sdlExtensions;
    sdlExtensions = (const char**)malloc(sdlExtensionCount*sizeof(const char**));

    for(int i = 0; i < sdlExtensionCount; i++)
    sdlExtensions[i] = (const char*)malloc(VK_MAX_EXTENSION_NAME_SIZE);
    

    SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, sdlExtensions);

    createInfo.enabledExtensionCount = sdlExtensionCount;
    createInfo.ppEnabledExtensionNames = sdlExtensions;

    createInfo.enabledLayerCount = 0;
    
    if(vkCreateInstance(&createInfo, NULL, &vkinst) != VK_SUCCESS){
        fprintf(stderr, "[%s] Failed to create instance",TAG);
        return EXIT_FAILURE; 
    }

    free(sdlExtensions);
    /*--------------------------------------------------------------*/
    /*                   Picking Physical Device                    */
    /*--------------------------------------------------------------*/
    vulkan_pick_physical_device();
    //Creating rendering surface
    vulkan_create_window_surface();
    //creating presentation queue
    VkDeviceCreateInfo vdci;
    vulkan_create_presentation_queue(&vdci);

    //free(qcip);
    return EXIT_SUCCESS;
}

int vulkan_create_window_surface(){
    if(!SDL_Vulkan_CreateSurface(
        window,
        (SDL_vulkanInstance)vkinst,
        (SDL_vulkanSurface*)&surface
    )){
        fprintf(stderr,"[%s] Failed to create surface!\n",TAG);
    }
    printf("[%s] Created surface Successfully!\n",TAG);
    return EXIT_SUCCESS;
}
int vulkan_create_presentation_queue(VkDeviceCreateInfo *vdci){
    struct QueueFamilyIndices indices;
    vulkan_has_suitable_queue_family(physical_device, &indices);
    if(!(indices.graphicsFamilyValid && indices.presentFamilyValid)){
        fprintf(stderr, "[%s] Failed to create the presentation queue!", TAG);
        return EXIT_FAILURE;
    }
    
    
    const size_t queueCreateInfosSize = 2;
    uint32_t uniqueQueueFamilies[] = {indices.graphicsFamily, indices.presentFamily};
    if(indices.graphicsFamily > indices.presentFamily){
        uniqueQueueFamilies[0] = indices.presentFamily;
        uniqueQueueFamilies[1] = indices.graphicsFamily;
    }

    VkDeviceQueueCreateInfo* queueCreateInfos;
    queueCreateInfos = (VkDeviceQueueCreateInfo*)malloc(queueCreateInfosSize*sizeof(VkDeviceQueueCreateInfo));
    float queuePriority = 1.0;

    for(int a = 0; a < queueCreateInfosSize; a++){
        VkDeviceQueueCreateInfo queueCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = uniqueQueueFamilies[a],
            .queueCount = 1,
            .pQueuePriorities = &queuePriority,
        };
        queueCreateInfos[a] = queueCreateInfo;
    }

    VkPhysicalDeviceFeatures deviceFeatures;
    
    VkDeviceCreateInfo dci = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = queueCreateInfosSize,
        .pQueueCreateInfos = queueCreateInfos,
        .pEnabledFeatures = NULL,
        .enabledLayerCount = 0,
        .ppEnabledExtensionNames = NULL,
    };
    /*vdci->sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    vdci->queueCreateInfoCount = queueCreateInfosSize;
    vdci->pQueueCreateInfos = queueCreateInfos;
    vdci->pEnabledFeatures = &deviceFeatures;
    vdci->enabledLayerCount = 0;*/

    if(vulkan_debug_result(vkCreateDevice(physical_device, &dci, NULL, &logical_device) != VK_SUCCESS)){
        fprintf(stderr, "[%s] Failed to create the logical device!", TAG);
        return EXIT_FAILURE;
    }

    vkGetDeviceQueue(logical_device, indices.graphicsFamily, 0, &graphics_queue);
    vkGetDeviceQueue(logical_device, indices.presentFamily, 0, &presentation_queue);

    return EXIT_SUCCESS;
}

static bool right_to_live(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT: return false; break;
            default: break;
        }
    }
    return true;
}

void vulkan_main_loop(){
    while(right_to_live()){

    }
}

int vulkan_pick_physical_device(){
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkinst, &deviceCount, NULL);

    if(deviceCount == 0){
        fprintf(stderr, "[%s] Failed to find any GPU!",TAG);
        return EXIT_FAILURE;
    }

    printf("[%s] Found %d devices.\n", TAG, deviceCount);

    VkPhysicalDevice* pdevs;
    pdevs = (VkPhysicalDevice*)malloc(deviceCount * sizeof(VkPhysicalDevice));
    if(pdevs == NULL){
        fprintf(stderr, "[%s] %s",TAG, ERR_MSG_MALLOC);
        return EXIT_FAILURE;
    }
    vkEnumeratePhysicalDevices(vkinst, &deviceCount, pdevs);

    int devscore = 0;
    for(int a = 0; a < deviceCount; a++){
        int tentative_score = vulkan_get_arbitrary_device_suitability(pdevs[a]);
        if(devscore <= tentative_score && vulkan_has_suitable_queue_family(pdevs[a], NULL)){
            devscore = tentative_score;
            physical_device = pdevs[a];
        }
    }

    if(devscore == 0)
    fprintf(stderr, "[%s] Selected device might not be suitable to run this program!",TAG);

    free(pdevs);

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physical_device, &deviceProperties);
    printf("[%s] Picked physical device: %s\n",TAG, deviceProperties.deviceName);
    
    return EXIT_SUCCESS;
}

int vulkan_cleanup(){
    //-----Vulkan----
    vkDestroySurfaceKHR(vkinst, surface, NULL);
    vkDestroyInstance(vkinst, NULL);
    //-----SDL-----
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void vulkan_dump_extention_info(){
    FILE *fp;
    fp = fopen(VULKAN_EXTENSION_INFO_LOG_FILE,"w+");

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);

    VkExtensionProperties* extprops;
    extprops = (VkExtensionProperties*)malloc(extensionCount * sizeof(VkExtensionProperties));
    
    if(extprops == NULL){
        fprintf(stderr, "[%s] %s",TAG, ERR_MSG_MALLOC);
    }else{
        vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extprops);
        for(int a = 0; a < extensionCount; a++){
            fprintf(fp, "%s\n", extprops[a].extensionName);
        }
        free(extprops);
    } 
    fclose(fp);
}