#include "../inc/vresult.h"
#include <vulkan/vulkan_core.h>

static const char* TAG = "VkResult Inspection";

VkResult vulkan_debug_result(VkResult result){
    switch (result) {
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: fprintf(stderr, "[%s] VK_ERROR_OUT_OF_DEVICE_MEMORY!\n", TAG); break;
        case VK_ERROR_INITIALIZATION_FAILED: fprintf(stderr, "[%s] VK_ERROR_INITIALIZATION_FAILED!\n", TAG); break;
        case VK_ERROR_EXTENSION_NOT_PRESENT: fprintf(stderr, "[%s] VK_ERROR_EXTENSION_NOT_PRESENT!\n", TAG); break;
        case VK_ERROR_FEATURE_NOT_PRESENT: fprintf(stderr, "[%s] VK_ERROR_FEATURE_NOT_PRESENT!\n", TAG); break;
        case VK_ERROR_TOO_MANY_OBJECTS: fprintf(stderr, "[%s] VK_ERROR_TOO_MANY_OBJECTS!\n", TAG); break;
        case VK_ERROR_DEVICE_LOST: fprintf(stderr, "[%s] VK_ERROR_DEVICE_LOST!\n", TAG); break;
        case VK_SUCCESS: fprintf(stderr, "[%s] VK_SUCCESS!\n", TAG); break;
        default: fprintf(stderr, "[%s] UNKNOWN! ID = %d\n", TAG); break;
    }
    return result;
}