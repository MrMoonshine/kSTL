//#include <vhelper.h>
#include "../inc/vhelper.h"
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vulkan/vulkan_core.h>

static GLFWwindow* window;
static VkInstance vkinst;
static VkPhysicalDevice physical_device = VK_NULL_HANDLE;
static const char *TAG = "Vulkan";

/*
* @brief inits the GLFW window
* @returns EXIT_SUCCESS or EXIT_FAILURE
*/
static int vulkan_init_window(){
    if(!glfwInit())
    return EXIT_FAILURE;

    if(!glfwVulkanSupported()){
        fprintf(stderr, "[%s] Vulkan is not supported by GLFW: Terminating...",TAG);
        return EXIT_FAILURE;
    }    

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(APP_WIDTH, APP_HEIGHT, APP_TITLE, NULL, NULL);

    if(!window){
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    return EXIT_SUCCESS;
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
    vulkan_init();
    vulkan_init_window();
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

    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo
    };

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;
    
    if(vkCreateInstance(&createInfo, NULL, &vkinst) != VK_SUCCESS){
        fprintf(stderr, "[%s] Failed to create instance",TAG);
        return EXIT_FAILURE; 
    }
    /*--------------------------------------------------------------*/
    /*                   Picking Physical Device                    */
    /*--------------------------------------------------------------*/
    vulkan_pick_physical_device();
    return EXIT_SUCCESS;
}

void vulkan_main_loop(){
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
    }
}

int vulkan_pick_physical_device(){
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkinst, &deviceCount, NULL);

    if(deviceCount == 0){
        fprintf(stderr, "[%s] Failed to find a suitable GPU!",TAG);
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
        if(devscore <= tentative_score){
            devscore = tentative_score;
            physical_device = pdevs[a];
        }
    }
    free(pdevs);

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physical_device, &deviceProperties);
    printf("[%s] Picked physical device: %s\n",TAG, deviceProperties.deviceName);
    
    return EXIT_SUCCESS;
}

int vulkan_cleanup(){
    //-----Vulkan----
    vkDestroyInstance(vkinst, NULL);
    //-----GLFW-----
    glfwDestroyWindow(window);
    glfwTerminate();
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