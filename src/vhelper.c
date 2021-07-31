//#include <vhelper.h>
#include "../inc/vhelper.h"

static GLFWwindow* window;
static VkInstance vkinst;
static const char *TAG = "VULKAN";

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

int vulkan_run(){
    vulkan_init();
    vulkan_init_window();
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
    return EXIT_SUCCESS;
}

int vulkan_main_loop(){
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
    }
}

int vulkan_cleanup(){
    //-----Vulkan----
    vkDestroyInstance(vkinst, NULL);
    //-----GLFW-----
    glfwDestroyWindow(window);
    glfwTerminate();
}